#pragma once

#include "GameState.hpp"

#include <memory>


class ScreenState;
class EnterHandler;
class ReturnHandler;

struct StateFrame
{
private:
    std::unique_ptr<GameState> state;

public:
    explicit StateFrame(std::unique_ptr<GameState> state);

    StateFrame(StateFrame&&) = default;
    StateFrame& operator=(StateFrame&&) = default;

    ScreenState* screenState = nullptr;
    EnterHandler* enterHandler = nullptr;
    ReturnHandler* returnHandler = nullptr;
};
