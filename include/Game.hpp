#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "StateFrame.hpp"
#include "FrameTransition.hpp"
#include "Settings.hpp"
#include "HallOfFameScores.hpp"
#include "RandomGenerator.hpp"
#include "GameContext.hpp"
#include "Renderer.hpp"


class Game
{
private:
    using FramesStack = std::vector<StateFrame>;

    Settings settings{};
    HallOfFameScores hallOfFameScores{};
    RandomGenerator randomGenerator{};

    GameContext context{
        settings,
        hallOfFameScores,
        randomGenerator
    };

    FramesStack frames;
    Renderer renderer;

    void enterCurrentFrame();

    void processTransition(FrameTransition transition);
    void applyTransition(NoneTransition) {}
    void applyTransition(QuitGameTransition);
    void applyTransition(ChangeStateTransition tr);
    void applyTransition(PushStateTransition tr);
    void applyTransition(ReturnTransition tr);

public:
    void run();

    template <typename State, typename... Args>
    void pushState(Args&&... args)
    {
        this->frames.emplace_back(std::make_unique<State>(this->context, std::forward<Args>(args)...));

        this->enterCurrentFrame();
    }
};
