#pragma once

#include "GameState.hpp"
#include "MenuScreenState.hpp"
#include "Settings.hpp"
#include "StateInterfaces.hpp"

#include <string>
#include <any>


class HallOfFameClearConfirmationStep final : public MenuScreenState, public Returns<bool> 
{
protected:
    virtual std::string getTitle() const override;

public:
    HallOfFameClearConfirmationStep(GameContext& context);

    virtual Screen getScreen() const override;
};

class LanguageSelectionState final : public MenuScreenState, public Returns<Language> 
{
protected:
    virtual std::string getTitle() const override;

public:
    LanguageSelectionState(GameContext& context);

    virtual Screen getScreen() const override;
};

class SettingsState final : public MenuScreenState, public ReturnHandler
{
private:
    std::string statusMessage;

    void clearHallOfFame();
    void insertHeader(Screen& screen) const;

protected:
    virtual std::string getTitle() const override;

public:
    SettingsState(GameContext& context);
    virtual FrameTransition handleReturn(std::any value) override;

    virtual Screen getScreen() const override;
};
