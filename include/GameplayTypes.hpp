#pragma once

#include "GameplayDifficulty.hpp"

#include <optional>
#include <variant>
#include <string_view>
#include <stdexcept>
#include <chrono>


struct StandardMode
{
    std::optional<int> maxAttempts;
};

struct NewGamePlusMode {};

using GameplayMode = std::variant<StandardMode, NewGamePlusMode>;

struct GameplayOptions
{
    GameplayDifficulty difficulty;
    GameplayMode mode;
};

enum class GameplayOutcome
{
    Victory,
    Defeat
};

[[nodiscard]] constexpr std::string_view toString(GameplayOutcome outcome)
{
    using enum GameplayOutcome;

    switch (outcome)
    {
    case Victory:
        return "Victory";

    case Defeat:
        return "Defeat";
    }

    throw std::invalid_argument("invalid GameplayOutcome value");
}

struct GameplayResult
{
    GameplayDifficulty difficulty;
    GameplayOutcome outcome;
    std::chrono::seconds gameDuration;
    int wrongAttempts;
    GameplayMode mode;
};
