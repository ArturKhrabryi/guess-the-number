#pragma once


struct Settings;
class HallOfFameScores;
class RandomGenerator;

struct GameContext
{
    Settings& settings;
    HallOfFameScores& hallOfFameScores;
    RandomGenerator& randomGenerator;
};
