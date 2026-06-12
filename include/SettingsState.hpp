#pragma once

#include "GameState.hpp"
#include "GameTypes.hpp"
#include <string>
#include <string_view>


class HallOfFameDeleteonConfirmationStep final : public GameState, public MenuScreenState, public Returns<bool> 
{
protected:
    virtual std::string_view getTitle() const override { return "Hall of fame deleteon confirmation"; }

public:
    HallOfFameDeleteonConfirmationStep(GameContext& context);

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
