#pragma once

#include <cstddef>
#include <stdexcept>
#include <string_view>


enum class GameplayDifficulty
{
    Easy, 
    Medium,
    Hard,

    Count
};

struct RandomNumberLimits
{
    int min;
    int max;
};

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
