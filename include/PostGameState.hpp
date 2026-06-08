#pragma once

#include "GameState.hpp"
#include "GameTypes.hpp"


class PostGameState final : public GameState, public ScreenState, public Returns<GameScoreAndDifficulty>
{
private:
    GameplayResult result;

public:
    PostGameState(GameplayResult result) : result(result) {}

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

