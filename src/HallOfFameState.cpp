#include "HallOfFameState.hpp"
#include "GameState.hpp"
#include "GameTypes.hpp"
#include "Screen.hpp"

#include <string>
#include <vector>
#include <cstddef>
#include <format>
#include <stdexcept>


bool HallOfFameDifficultySelectionState::isVisible(GameplayDifficulty difficulty) const
{
    return !this->getContext().hallOfFameScores.isEmpty(difficulty);
}

HallOfFameDifficultySelectionState::HallOfFameDifficultySelectionState(GameContext& context) : GameState(context)
{
    for (std::size_t difficultyIndex = 0; difficultyIndex < getDifficultyCount(); ++difficultyIndex)
    {
        const auto difficulty = difficultyFromIndex(difficultyIndex);

        this->addMenuItem(MenuItem{
            .text = std::string{ toString(difficulty) },
            .handler = [difficulty] { return HallOfFameDifficultySelectionState::makeReturn(difficulty); },
            .isVisible = [this, difficulty] { return this->isVisible(difficulty); }
        });
    }
}

Screen HallOfFameDisplayState::getScreen() const
{
    auto title = std::format("TOP5 scores for {} difficulty", toString(this->difficulty));

    Screen screen{
        std::move(title),
        "Enter any input to return to the main menu"
    };

    for (const auto& gameScore : this->getContext().hallOfFameScores.getGameScores(this->difficulty))
    {
        auto text = std::format(
            "{}: {} attempt{} in {}",
            gameScore.name,
            gameScore.attempts,
            gameScore.attempts == 1 ? "" : "s",
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
