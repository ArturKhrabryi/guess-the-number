#include "MainMenuState.hpp"
#include "GameState.hpp"
#include "GameTypes.hpp"
#include "OptionsSelectionState.hpp"
#include "PostGameState.hpp"
#include "TOP5DisplayState.hpp"
#include "GameplayState.hpp"

#include <algorithm>
#include <any>
#include <cstddef>
#include <memory>
#include <stdexcept>


void MainMenuState::addGameScore(GameScoreAndDifficulty scoreAndDifficulty)
{
    auto difficultyIndex = toIndex(scoreAndDifficulty.difficulty);
    auto& scoresForDifficulty = this->scores[difficultyIndex];

    scoresForDifficulty.push_back(std::move(scoreAndDifficulty.score));
    
    std::ranges::sort(scoresForDifficulty, {}, &GameScore::attempts);

    constexpr std::size_t maxSize = 5;
    if (scoresForDifficulty.size() > maxSize)
        scoresForDifficulty.resize(maxSize);
}

bool MainMenuState::isEmptyScores() const
{
    for (const auto& vec : this->scores)
    {
        if (!vec.empty())
            return false;
    }

    return true;
}

MainMenuState::MainMenuState()
{
    this->addMenuItem(MenuItem{
        .text = "Start new game",
        .handler = [] {
            return PushStateTransition{ .nextState = std::make_unique<OptionsSelectionState>() };
        }
    });

    this->addMenuItem(MenuItem{
        .text = "TOP5",
        .handler = [this] {
            return PushStateTransition{ .nextState = std::make_unique<TOP5DifficultySelectionState>(this->scores) };
        },
        .isVisible = [this] {
            return !this->isEmptyScores();
        }
    });

    this->addMenuItem(MenuItem{
        .text = "Quit the game",
        .handler = [] {
            return QuitGameTransition{};
        }
    });
}

FrameTransition MainMenuState::handleReturn(std::any value)
{
    if (auto gameplayOptions = std::any_cast<OptionsSelectionState::ReturnType>(&value))
        return PushStateTransition{ .nextState = std::make_unique<GameplayState>(*gameplayOptions) };

    if (auto gameplayResult = std::any_cast<GameplayState::ReturnType>(&value))
        return PushStateTransition{ .nextState = std::make_unique<PostGameState>(*gameplayResult) };

    if (auto gameScoreAndDifficulty = std::any_cast<PostGameState::ReturnType>(&value))
    {
        this->addGameScore(std::move(*gameScoreAndDifficulty));

        return NoneTransition{};
    }

    if (auto difficulty = std::any_cast<TOP5DifficultySelectionState::ReturnType>(&value))
    {
        auto difficultyIndex = toIndex(*difficulty);

        return PushStateTransition{ .nextState = std::make_unique<TOP5DisplayState>(this->scores[difficultyIndex], *difficulty) };
    }

    throw std::logic_error("Unknown return type");
}
