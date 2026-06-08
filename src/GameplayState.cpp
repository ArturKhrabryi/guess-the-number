#include "GameplayState.hpp"
#include "GameTypes.hpp"
#include "InputHandler.hpp"
#include "Screen.hpp"

#include <array>
#include <limits>
#include <string>
#include <string_view>


std::string_view GameplayState::getRandomGreaterNumberPhrase() const
{
    using namespace std::string_view_literals;

    static constexpr std::array phrases = {
        "The secret number lives upstairs"sv,
        "You're shooting below the mark"sv,
        "Not quite - the answer is higher"sv,
        "You're under the target"sv,
        "The number is hiding above that"sv
    };

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

std::string_view GameplayState::getRandomSmallerNumberPhrase() const
{
    using namespace std::string_view_literals;

    static constexpr std::array phrases = {
        "Too high - dial it down"sv,
        "Take that guess down a notch"sv,
        "You overshot it. Pull back"sv,
        "Bring it back down to earth"sv,
        "Easy there, that's too much"sv
    };

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

void GameplayState::setRandomNumber(GameplayDifficulty difficulty)
{
    const auto [min, max] = getRandomNumberLimits(difficulty);
    this->randomNumber = randomGenerator.generate(min, max);
}

std::string GameplayState::constructAttemptsIndicatorText() const
{
    std::string attemptsIndicatorText = "Attempt #" + std::to_string(this->wrongAttempts + 1);
    if (this->options.maxAttempts)
        attemptsIndicatorText += "/" + std::to_string(*this->options.maxAttempts);

    return attemptsIndicatorText;
}

GameplayState::GameplayState(GameplayOptions options) :
    options(options)
{
    this->setRandomNumber(this->options.difficulty);
}

FrameTransition GameplayState::handleEvent(const Event& event)
{
    auto guessedNumberOptional = InputHandler::toNumber<int>(event.value);
    if (!guessedNumberOptional)
    {
        this->statusMessage = "Invalid input";

        return NoneTransition{};
    }

    auto guessedNumber = *guessedNumberOptional;

    auto randomNumberLimits = getRandomNumberLimits(this->options.difficulty);
    if (guessedNumber < randomNumberLimits.min || guessedNumber > randomNumberLimits.max)
    {
        this->statusMessage = "Do you really want to enter a number outside the difficulty's range?";

        return NoneTransition{};
    }

    if (guessedNumber == this->randomNumber)
        return ReturnTransition{ .value = ReturnType{
            .difficulty = this->options.difficulty,
            .outcome = GameplayOutcome::Victory,
            .wrongAttempts = this->wrongAttempts
        } };

    else
    {
        ++this->wrongAttempts;

        bool isChallengeMode = this->options.maxAttempts.has_value();
        if (isChallengeMode && this->wrongAttempts >= *this->options.maxAttempts)
            return ReturnTransition{ .value = ReturnType{
                .difficulty = this->options.difficulty,
                .outcome = GameplayOutcome::Defeat,
                .wrongAttempts = this->wrongAttempts
            } };

        if (guessedNumber < this->randomNumber)
            this->statusMessage = this->getRandomGreaterNumberPhrase();

        else
            this->statusMessage = this->getRandomSmallerNumberPhrase();
    }

    return NoneTransition{};
}

Screen GameplayState::getScreen() const
{
    Screen screen{
        "Gameplay",
        "Try to guess the number:"
    };

    screen.header.push_back(TextElement{
        .text = "Difficulty: " + std::string{ toString(this->options.difficulty) }
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

#if !defined(NDEBUG)
    screen.body.push_back(TextElement{
        .text = std::string("Random number: ") + std::to_string(this->randomNumber),
        .role = TextRole::Debug
    });
#endif

    return screen;
}
