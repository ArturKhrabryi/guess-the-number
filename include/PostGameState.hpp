#pragma once

#include "GameState.hpp"
#include "GameTypes.hpp"


class PostGameState final : public GameState, public ScreenState
{
private:
    GameplayResult result;

public:
    PostGameState(GameContext& context, GameplayResult result) : GameState(context), result(result) {}

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

