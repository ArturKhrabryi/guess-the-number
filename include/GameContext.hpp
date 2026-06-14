#pragma once


struct Settings;
class HallOfFameScores;
class RandomGenerator;
class Translator;

struct GameContext
{
    Settings& settings;
    HallOfFameScores& hallOfFameScores;
    RandomGenerator& randomGenerator;
    Translator& translator;
};
