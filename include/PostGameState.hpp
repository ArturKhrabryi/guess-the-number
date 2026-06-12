#pragma once

#include "GameState.hpp"
#include "GameplayTypes.hpp"
#include "StateInterfaces.hpp"

class PostGameState final : public GameState, public ScreenState
{
private:
    GameplayResult result;

public:
    PostGameState(GameContext& context, GameplayResult result) : GameState(context), result(result) {}

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

