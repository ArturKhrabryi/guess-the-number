#pragma once

#include "GameState.hpp"

#include <any>
#include <memory>
#include <variant>


struct NoneTransition {};
struct QuitGameTransition {};

struct ChangeStateTransition
{
    std::unique_ptr<GameState> nextState{}; 
};

struct PushStateTransition
{
    std::unique_ptr<GameState> nextState{}; 
};

struct VoidReturn{};

struct ReturnTransition
{
    std::any value = VoidReturn{};
};

using FrameTransition = std::variant<
    NoneTransition,
    QuitGameTransition,
    ChangeStateTransition,
    PushStateTransition,
    ReturnTransition
>;
