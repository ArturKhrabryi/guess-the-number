#pragma once

#include <string>
#include <any>

#include "GameState.hpp"
#include "StateInterfaces.hpp"
#include "MenuScreenState.hpp"


class MainMenuState final : public MenuScreenState, public ReturnHandler
{
protected:
    virtual std::string getTitle() const override;

public:
    MainMenuState(GameContext& context);
    virtual FrameTransition handleReturn(std::any value) override;
};
