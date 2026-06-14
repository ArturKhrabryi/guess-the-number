#pragma once

#include "GameState.hpp"
#include "StateInterfaces.hpp"
#include "MenuScreenState.hpp"
#include "GameplayDifficulty.hpp"
#include "GameplayTypes.hpp"

#include <string>
#include <any>


class DifficultySelectionStep final : public MenuScreenState, public Returns<GameplayDifficulty>
{
private:
    struct DifficultyItem
    {
        GameplayDifficulty difficulty;
        std::string name;
    };

protected:
    virtual std::string getTitle() const override;

public:
    DifficultySelectionStep(GameContext& context);
    virtual Screen getScreen() const override;
};

class GameplayModeSelectionStep final : public MenuScreenState, public Returns<GameplayMode>
{
protected:
    virtual std::string getTitle() const override;

public:
    GameplayModeSelectionStep(GameContext& context);
    virtual Screen getScreen() const override;
};

class ChallengeModeSelectionStep final : public MenuScreenState, public Returns<bool>
{
protected:
    virtual std::string getTitle() const override;

public:
    ChallengeModeSelectionStep(GameContext& context);
    virtual Screen getScreen() const override;
};

class MaxAttemptsSelectionStep final : public GameState, public ScreenProvider, public Returns<int>
{
public:
    using GameState::GameState;

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

class OptionsSelectionState final : public GameState, public EnterHandler, public ReturnHandler, public Returns<GameplayOptions>
{
private:
    ReturnType options{};

public:
    using GameState::GameState;

    virtual FrameTransition onEnter() override;
    virtual FrameTransition handleReturn(std::any value) override;
};
