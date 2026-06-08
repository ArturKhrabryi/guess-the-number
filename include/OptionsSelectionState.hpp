#pragma once

#include "GameState.hpp"
#include "GameTypes.hpp"

#include <string>
#include <vector>


class DifficultySelectionStep final : public GameState, public MenuScreenState, public Returns<GameplayDifficulty>
{
private:
    struct DifficultyItem
    {
        GameplayDifficulty difficulty;
        std::string name;
    };

    static DifficultyItem constructDifficultyItem(GameplayDifficulty difficulty);
    static std::vector<DifficultyItem> constructDifficultyItems();

protected:
    virtual std::string_view getTitle() const override { return "Difficulty selection"; }

public:
    DifficultySelectionStep();
    virtual Screen getScreen() const override;
};

class ChallengeModeSelectionStep final : public GameState, public MenuScreenState, public Returns<bool>
{
protected:
    virtual std::string_view getTitle() const override { return "Enabling challenge mode"; }

public:
    ChallengeModeSelectionStep();
    virtual Screen getScreen() const override;
};

class MaxAttemptsSelectionStep final : public GameState, public ScreenState, public Returns<int>
{
public:
    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

class OptionsSelectionState final : public GameState, public EnterHandler, public ReturnHandler, public Returns<GameplayOptions>
{
private:
    ReturnType options{};

public:
    virtual FrameTransition onEnter() override;
    virtual FrameTransition handleReturn(std::any value) override;
};
