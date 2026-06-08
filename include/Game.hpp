#pragma once

#include <memory>
#include <vector>

#include "GameState.hpp"
#include "Renderer.hpp"


class Game
{
private:
    using FramesStack = std::vector<StateFrame>;

    FramesStack frames;
    Renderer renderer;

    void enterCurrentFrame();

    void processTransition(FrameTransition transition);
    void applyTransition(NoneTransition) {}
    void applyTransition(QuitGameTransition);
    void applyTransition(ChangeStateTransition tr);
    void applyTransition(PushStateTransition tr);
    void applyTransition(PopStateTransition);
    void applyTransition(ReturnTransition tr);

public:
    Game(std::unique_ptr<GameState> initialState);

    void run();
};
