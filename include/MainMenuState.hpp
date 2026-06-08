#pragma once

#include <string_view>

#include "GameState.hpp"
#include "GameTypes.hpp"


class MainMenuState final : public GameState, public MenuScreenState, public ReturnHandler
{
private:
    GameScoresByDifficulty scores{};

    void addGameScore(GameScoreAndDifficulty scoreAndDifficulty);
    bool isEmptyScores() const;

protected:
    virtual std::string_view getTitle() const override { return "Main menu"; } 

public:
    MainMenuState();
    virtual FrameTransition handleReturn(std::any value) override;
};
