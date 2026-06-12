#include "HallOfFameScores.hpp"

#include <algorithm>
#include <tuple>
#include <utility>


void HallOfFameScores::addGameScore(GameScore score, GameplayDifficulty difficulty)
{
    auto difficultyIndex = toIndex(difficulty);
    auto& scoresForDifficulty = this->scores[difficultyIndex];

    scoresForDifficulty.push_back(std::move(score));
    
    std::ranges::sort(
        scoresForDifficulty,
        std::less<>{},
        [](const GameScore& score) {
            return std::tuple{
                score.attempts,
                score.gameDuration
            };
        } 
    );

    constexpr std::size_t maxSize = 5;
    if (scoresForDifficulty.size() > maxSize)
        scoresForDifficulty.resize(maxSize);
}

const GameScores& HallOfFameScores::getGameScores(GameplayDifficulty difficulty) const
{
    auto difficultyIndex = toIndex(difficulty);

    return this->scores[difficultyIndex];
}

bool HallOfFameScores::isEmpty() const
{
    for (const auto& scoresForDifficulty : this->scores)
    {
        if (!scoresForDifficulty.empty())
            return false;
    }

    return true;
}

bool HallOfFameScores::isEmpty(GameplayDifficulty difficulty) const
{
    auto difficultyIndex = toIndex(difficulty);

    return this->scores[difficultyIndex].empty();
}

void HallOfFameScores::clear()
{
    for (auto& vec : this->scores)
        vec.clear();
}
