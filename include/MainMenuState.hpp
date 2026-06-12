#pragma once

#include <string_view>
#include <any>

#include "GameState.hpp"
#include "StateInterfaces.hpp"
#include "MenuScreenState.hpp"


class MainMenuState final : public GameState, public MenuScreenState, public ReturnHandler
{
protected:
    virtual std::string_view getTitle() const override { return "Main menu"; } 

public:
    MainMenuState(GameContext& context);
    virtual FrameTransition handleReturn(std::any value) override;
};
