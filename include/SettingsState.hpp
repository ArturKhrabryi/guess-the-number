#pragma once

#include "GameState.hpp"
#include "MenuScreenState.hpp"
#include "StateInterfaces.hpp"

#include <string>
#include <string_view>
#include <any>


class HallOfFameClearConfirmationStep final : public GameState, public MenuScreenState, public Returns<bool> 
{
protected:
    virtual std::string_view getTitle() const override { return "Hall of fame deleteon confirmation"; }

public:
    HallOfFameClearConfirmationStep(GameContext& context);

    virtual Screen getScreen() const override;
};

class SettingsState final : public GameState, public MenuScreenState, public ReturnHandler
{
private:
    std::string statusMessage;

    void deleteHallOfFame();
    void insertHeader(Screen& screen) const;

protected:
    virtual std::string_view getTitle() const override { return "Settings"; }

public:
    SettingsState(GameContext& context);
    virtual FrameTransition handleReturn(std::any value) override;

    virtual Screen getScreen() const override;
};
