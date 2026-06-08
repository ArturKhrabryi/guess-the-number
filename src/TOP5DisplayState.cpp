#include "TOP5DisplayState.hpp"
#include "GameState.hpp"
#include "GameTypes.hpp"
#include "Screen.hpp"

#include <string>
#include <vector>
#include <cstddef>
#include <format>


bool TOP5DifficultySelectionState::isVisible(GameplayDifficulty difficulty) const
{
    auto difficultyIndex = toIndex(difficulty);

    return !this->scores[difficultyIndex].empty();
}

TOP5DifficultySelectionState::TOP5DifficultySelectionState(const GameScoresByDifficulty& scores) : scores(scores)
{
    for (std::size_t difficultyIndex = 0; difficultyIndex < getDifficultyCount(); ++difficultyIndex)
    {
        const auto difficulty = difficultyFromIndex(difficultyIndex);

        this->addMenuItem(MenuItem{
            .text = std::string{ toString(difficulty) },
            .handler = [difficulty] { return TOP5DifficultySelectionState::makeReturn(difficulty); },
            .isVisible = [this, difficulty] { return this->isVisible(difficulty); }
        });
    }
}

Screen TOP5DisplayState::getScreen() const
{
    auto title = std::format("TOP5 scores for {} difficulty", toString(this->difficulty));

    Screen screen{
        std::move(title),
        "Enter any input to return to the main menu"
    };

    for (const auto& gameScore : this->scores)
    {
        auto text = std::format("{}: {} attempts", gameScore.name, gameScore.attempts);
        screen.body.push_back(TextElement{ .text = std::move(text), .role = TextRole::Normal });
    }

    return screen;
}

FrameTransition TOP5DisplayState::handleEvent(const Event&)
{
    return PopStateTransition{}; 
}
