#pragma once

#include "RandomGenerator.hpp"

#include <chrono>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <array>
#include <cstddef>


enum class GameplayDifficulty
{
    Easy, 
    Medium,
    Hard,

    Count
};

struct StandardMode
{
    std::optional<int> maxAttempts;
};

struct NewGamePlusMode {};

using GameplayMode = std::variant<StandardMode, NewGamePlusMode>;

[[nodiscard]] constexpr std::size_t getDifficultyCount()
{
    return static_cast<std::size_t>(GameplayDifficulty::Count);
}

[[nodiscard]] constexpr GameplayDifficulty difficultyFromIndex(std::size_t index)
{
    if (index >= getDifficultyCount())
        throw std::out_of_range("difficulty index out of range");

    return static_cast<GameplayDifficulty>(index);
}

[[nodiscard]] constexpr std::size_t toIndex(GameplayDifficulty difficulty)
{
    auto index = static_cast<std::size_t>(difficulty);
    if (index >= getDifficultyCount())
        throw std::invalid_argument("invalid GameplayDifficulty value");

    return index;
}

[[nodiscard]] constexpr std::string_view toString(GameplayDifficulty difficulty)
{
    using enum GameplayDifficulty;

    switch (difficulty)
    {
    case Easy:
        return "Easy";

    case Medium:
        return "Medium";

    case Hard:
        return "Hard";

    case Count:
        break;
    }

    throw std::invalid_argument("invalid GameplayDifficulty value");
}

struct RandomNumberLimits
{
    int min;
    int max;
};

[[nodiscard]] constexpr RandomNumberLimits getRandomNumberLimits(GameplayDifficulty difficulty)
{
    using enum GameplayDifficulty;

    switch (difficulty)
    {
    case Easy:
        return { .min = 1, .max = 50 };

    case Medium:
        return { .min = 1, .max = 100 };

    case Hard:
        return { .min = 1, .max = 250 };

    case Count:
        break;
    }

    throw std::invalid_argument("invalid GameplayDifficulty value");
}

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

struct GameScore
{
    std::string name;
    int attempts;
    std::chrono::seconds gameDuration;
    bool isNewGamePlus;
};

using GameScores = std::vector<GameScore>;

class HallOfFameScores
{
private:
    std::array<GameScores, getDifficultyCount()> scores{};

public:
    void addGameScore(GameScore score, GameplayDifficulty difficulty);
    const GameScores& getGameScores(GameplayDifficulty difficulty) const;
    bool isEmpty() const;
    bool isEmpty(GameplayDifficulty difficulty) const;
    void clear();
};

enum class Language
{
    English,
    Polish
};

[[nodiscard]] constexpr std::string_view toString(Language language)
{
    using enum Language;

    switch (language)
    {
    case English:
        return "English";

    case Polish:
        return "Polish";
    }

    throw std::invalid_argument("invalid Language value");
}

struct Settings
{
    Language language = Language::English;
    bool askChallengeMode = true;
};

struct GameContext
{
    Settings settings;
    HallOfFameScores hallOfFameScores;
    RandomGenerator randomGenerator{};
};
