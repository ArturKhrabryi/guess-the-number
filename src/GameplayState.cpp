#include "GameplayState.hpp"
#include "GameContext.hpp"
#include "InputHandler.hpp"
#include "RandomGenerator.hpp"
#include "Screen.hpp"
#include "Translator.hpp"

#include <array>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>


std::chrono::seconds GameplayState::getElapsedTime() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - this->round.startTime);
}

int GameplayState::generateRandomNumber()
{
    const auto [min, max] = getRandomNumberLimits(this->options.difficulty);

    return this->getContext().randomGenerator.generate(min, max);
}

std::string_view GameplayState::selectRandomGreaterNumberPhrase()
{
    using namespace std::string_view_literals;

    static constexpr std::array phrases = {
        "The secret number lives upstairs"sv,
        "You're shooting below the mark"sv,
        "Not quite - the answer is higher"sv,
        "You're under the target"sv,
        "The number is hiding above that"sv
    };

    auto& randomGenerator = this->getContext().randomGenerator;
    static std::size_t lastIndex = std::numeric_limits<std::size_t>::max();
    auto randomPhraseIndex = randomGenerator.generate<std::size_t>(0, phrases.size() - 1);
    if constexpr (phrases.size() > 1)
    {
        while (randomPhraseIndex == lastIndex)
            randomPhraseIndex = randomGenerator.generate<std::size_t>(0, phrases.size() - 1);
    }

    lastIndex = randomPhraseIndex;

    return phrases[randomPhraseIndex];
}

std::string_view GameplayState::selectRandomSmallerNumberPhrase()
{
    using namespace std::string_view_literals;

    static constexpr std::array phrases = {
        "Too high - dial it down"sv,
        "Take that guess down a notch"sv,
        "You overshot it. Pull back"sv,
        "Bring it back down to earth"sv,
        "Easy there, that's too much"sv
    };

    auto& randomGenerator = this->getContext().randomGenerator;
    static std::size_t lastIndex = std::numeric_limits<std::size_t>::max();
    auto randomPhraseIndex = randomGenerator.generate<std::size_t>(0, phrases.size() - 1);
    if constexpr (phrases.size() > 1)
    {
        while (randomPhraseIndex == lastIndex)
            randomPhraseIndex = randomGenerator.generate<std::size_t>(0, phrases.size() - 1);
    }

    lastIndex = randomPhraseIndex;

    return phrases[randomPhraseIndex];
}

std::string GameplayState::selectStatusMessage(GuessStatus status)
{
    using enum GuessStatus;

    const auto& translator = this->getContext().translator;

    switch (status)
    {
    case Invalid:
        return translator.translateString("Please make a guess using a valid number");

    case OutOfRange:
        return translator.translateString("Do you really want to enter a number outside the difficulty's range?");

    case TooSmall:
        return translator.translateString(this->selectRandomGreaterNumberPhrase());

    case TooLarge:
        return translator.translateString(this->selectRandomSmallerNumberPhrase());

    case Rerolled:
        return translator.translateString("The secret number has been rerolled");
    }

    throw std::logic_error("Invalid GuessStatus value");
}

std::string GameplayState::constructAttemptsIndicatorText() const
{
    const auto& translator = this->getContext().translator;
    const auto attempt = this->round.wrongAttempts + 1;

    if (!this->isChallengeMode())
        return translator.format("Attempt #{}", attempt);

    return translator.format("Attempt #{}/{}", attempt, this->getMaxAttempts());
}

std::string GameplayState::constructAttemptsUntilRerollText() const
{
    const auto& translator = this->getContext().translator;
    const int attemptsLeft = this->getAttemptsUntilReroll();

    if (attemptsLeft == 1)
        return translator.translateString("You have one last try before the reroll");

    return translator.format("Reroll in {} attempts", attemptsLeft);
}

FrameTransition GameplayState::finish(GameplayOutcome outcome) const
{
    return GameplayState::makeReturn(ReturnType{
        .difficulty = this->options.difficulty,
        .outcome = outcome,
        .gameDuration = this->getElapsedTime(),
        .wrongAttempts = this->round.wrongAttempts,
        .mode = this->options.mode
    });
}

bool GameplayState::isNewGamePlusMode() const
{
    return std::holds_alternative<NewGamePlusMode>(this->options.mode);
}

bool GameplayState::isChallengeMode() const
{
    const auto* standardMode = std::get_if<StandardMode>(&this->options.mode);

    return standardMode != nullptr && standardMode->maxAttempts.has_value();
}

int GameplayState::getMaxAttempts() const
{
    return std::get<StandardMode>(this->options.mode).maxAttempts.value();
}

int GameplayState::getRerollInterval() const
{
    using enum GameplayDifficulty;

    switch (this->options.difficulty)
    {
    case Easy:
        return 8;

    case Medium:
        return 7;

    case Hard: 
        return 6;

    case Count:
        break;
    }

    throw std::logic_error("Invalid GameplayDifficulty value");
}

int GameplayState::getAttemptsUntilReroll() const
{
    int interval = this->getRerollInterval();

    return interval - this->round.wrongAttempts % interval;
}

void GameplayState::reroll()
{
    auto& targetNumber = this->round.targetNumber;
    int previousTargetNumber = targetNumber;

    while (previousTargetNumber == targetNumber)
        targetNumber = this->generateRandomNumber();
}

GameplayState::GameplayState(GameContext& context, GameplayOptions options) :
    GameState(context),
    options(options),
    round{ .targetNumber = this->generateRandomNumber() }
{}

FrameTransition GameplayState::handleEvent(const Event& event)
{
    auto guessedNumberOptional = InputHandler::toNumber<int>(event.value);
    if (!guessedNumberOptional)
    {
        this->statusMessage = this->selectStatusMessage(GuessStatus::Invalid);

        return NoneTransition{};
    }

    auto guessedNumber = *guessedNumberOptional;

    auto randomNumberLimits = getRandomNumberLimits(this->options.difficulty);
    if (bool isNotInLimits = guessedNumber < randomNumberLimits.min || guessedNumber > randomNumberLimits.max)
    {
        this->statusMessage = this->selectStatusMessage(GuessStatus::OutOfRange);

        return NoneTransition{};
    }

    if (guessedNumber == this->round.targetNumber)
        return this->finish(GameplayOutcome::Victory);

    ++this->round.wrongAttempts;

    if (bool isOutOfAttempts = this->isChallengeMode() && this->round.wrongAttempts >= this->getMaxAttempts())
        return this->finish(GameplayOutcome::Defeat);

    if (bool shouldReroll = this->isNewGamePlusMode() && this->round.wrongAttempts % this->getRerollInterval() == 0)
    {
        this->reroll();
        this->statusMessage = this->selectStatusMessage(GuessStatus::Rerolled);

        return NoneTransition{};
    }

    if (guessedNumber < this->round.targetNumber)
        this->statusMessage = this->selectStatusMessage(GuessStatus::TooSmall);

    else
        this->statusMessage = this->selectStatusMessage(GuessStatus::TooLarge);

    return NoneTransition{};
}

Screen GameplayState::getScreen() const
{
    const auto& translator = this->getContext().translator;

    Screen screen{
        translator.translateString("Gameplay"),
        translator.translateString("Try to guess the number")
    };

    screen.header.push_back(TextElement{
        .text = translator.format("Difficulty: {}", toString(this->options.difficulty))
    });

    if (!this->statusMessage.empty())
    {
        screen.body.push_back(TextElement{
            .text = this->statusMessage
        });
    }

    screen.body.push_back(TextElement{
        .text = this->constructAttemptsIndicatorText()
    });

    if (this->isNewGamePlusMode())
    {
        screen.body.push_back(TextElement{
            .text = this->constructAttemptsUntilRerollText()
        });
    }

#if !defined(NDEBUG)
    screen.body.push_back(TextElement{
        .text = translator.format("Random number: {}", this->round.targetNumber),
        .role = TextRole::Debug
    });
#endif

    return screen;
}
