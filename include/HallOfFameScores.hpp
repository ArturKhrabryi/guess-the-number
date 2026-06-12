#pragma once

#include "GameplayDifficulty.hpp"

#include <chrono>
#include <string>
#include <vector>
#include <array>


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
