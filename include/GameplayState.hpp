#pragma once

#include "GameState.hpp"
#include "RandomGenerator.hpp"
#include "GameTypes.hpp"

#include <string>
#include <string_view>


class GameplayState final : public GameState, public ScreenState, public Returns<GameplayResult>
{
private:
    int randomNumber{};

    int wrongAttempts = 0;
    GameplayOptions options;

    std::string statusMessage{};

    inline static RandomGenerator randomGenerator{};

    void setRandomNumber(GameplayDifficulty difficulty);

    std::string_view getRandomGreaterNumberPhrase() const;
    std::string_view getRandomSmallerNumberPhrase() const;

    std::string constructAttemptsIndicatorText() const;

public:
    GameplayState(GameplayOptions options);

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

