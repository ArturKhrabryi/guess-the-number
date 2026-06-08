#pragma once

#include "GameState.hpp"
#include "GameTypes.hpp"


class TOP5DifficultySelectionState final : public GameState, public MenuScreenState, public Returns<GameplayDifficulty>
{
private:
    const GameScoresByDifficulty& scores;

    bool isVisible(GameplayDifficulty difficulty) const;

protected:
    std::string_view getTitle() const override { return "Choose the difficulty to view TOP5"; }

public:
    TOP5DifficultySelectionState(const GameScoresByDifficulty& scores);
};

class TOP5DisplayState final : public GameState, public ScreenState
{
private:
    const GameScores& scores;
    GameplayDifficulty difficulty;

public:
    TOP5DisplayState(const GameScores& scores, GameplayDifficulty difficulty) :
        scores(scores),
        difficulty(difficulty)
    {}

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

