#pragma once

#include "GameState.hpp"
#include "StateInterfaces.hpp"
#include "GameplayDifficulty.hpp"
#include "MenuScreenState.hpp"

#include <any>
#include <string_view>


class HallOfFameDifficultySelectionState final : public GameState, public MenuScreenState, public Returns<GameplayDifficulty>
{
private:
    bool isVisible(GameplayDifficulty difficulty) const;

protected:
    virtual std::string_view getTitle() const override { return "Choose the difficulty to view TOP5"; }

public:
    HallOfFameDifficultySelectionState(GameContext& context);
};

class HallOfFameDisplayState final : public GameState, public ScreenState
{
private:
    GameplayDifficulty difficulty;

public:
    HallOfFameDisplayState(GameContext& context, GameplayDifficulty difficulty) :
        GameState(context),
        difficulty(difficulty)
    {}

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

class HallOfFameState final : public GameState, public EnterHandler, public ReturnHandler
{
public:
    using GameState::GameState;

    virtual FrameTransition onEnter() override;
    virtual FrameTransition handleReturn(std::any value) override;
};
