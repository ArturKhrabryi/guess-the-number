#include "HallOfFameState.hpp"
#include "GameContext.hpp"
#include "HallOfFameScores.hpp"
#include "Screen.hpp"
#include "Translator.hpp"

#include <string>
#include <vector>
#include <cstddef>
#include <stdexcept>


bool HallOfFameDifficultySelectionState::isVisible(GameplayDifficulty difficulty) const
{
    return !this->getContext().hallOfFameScores.isEmpty(difficulty);
}

std::string HallOfFameDifficultySelectionState::getTitle() const
{
    return this->getContext().translator.translateString("Choose the difficulty to view TOP5");
}

HallOfFameDifficultySelectionState::HallOfFameDifficultySelectionState(GameContext& context) : MenuScreenState(context)
{
    for (std::size_t difficultyIndex = 0; difficultyIndex < getDifficultyCount(); ++difficultyIndex)
    {
        const auto difficulty = difficultyFromIndex(difficultyIndex);

        this->addMenuItem(MenuItem{
            .textProvider = [this, difficulty] {
                const auto& translator = this->getContext().translator;

                return translator.translateString(toString(difficulty));
            },
            .handler = [difficulty] { return HallOfFameDifficultySelectionState::makeReturn(difficulty); },
            .isVisible = [this, difficulty] { return this->isVisible(difficulty); }
        });
    }
}

Screen HallOfFameDisplayState::getScreen() const
{
    const auto& translator = this->getContext().translator;

    auto title = translator.format(
        "TOP5 scores for {} difficulty",
        toString(this->difficulty)
    );

    Screen screen{
        std::move(title),
        translator.translateString("Enter any input to return to the main menu")
    };

    for (const auto& gameScore : this->getContext().hallOfFameScores.getGameScores(this->difficulty))
    {
        const auto attemptsText = gameScore.attempts == 1 ?
            translator.translateString("1 attempt") :
            translator.format("{} attempts", gameScore.attempts);

        auto text = gameScore.isNewGamePlus ?
            translator.format(
                "{}: {} in {} {}",
                translator.notTranslate(gameScore.name),
                translator.notTranslate(attemptsText),
                gameScore.gameDuration,
                translator.notTranslate("NG+")
            ) :
            translator.format(
                "{}: {} in {}",
                translator.notTranslate(gameScore.name),
                translator.notTranslate(attemptsText),
                gameScore.gameDuration        
            );

        screen.body.push_back(TextElement{ .text = std::move(text) });
    }

    return screen;
}

FrameTransition HallOfFameDisplayState::handleEvent(const Event&)
{
    return ReturnTransition{}; 
}

FrameTransition HallOfFameState::onEnter()
{
    return PushStateTransition{ .nextState = this->makeState<HallOfFameDifficultySelectionState>() };
}

FrameTransition HallOfFameState::handleReturn(std::any value)
{
    if (ReturnHandler::isVoid(value))
        return ReturnTransition{}; 

    if (auto* difficulty = std::any_cast<HallOfFameDifficultySelectionState::ReturnType>(&value))
        return PushStateTransition{ .nextState = this->makeState<HallOfFameDisplayState>(*difficulty) };

    throw std::logic_error("Unknown return type");
}
