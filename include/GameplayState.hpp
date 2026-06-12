#pragma once

#include "GameState.hpp"
#include "GameTypes.hpp"

#include <chrono>
#include <string>
#include <string_view>


class GameplayState final : public GameState, public ScreenState, public Returns<GameplayResult>
{
private:
    using Clock = std::chrono::steady_clock;

    enum class GuessStatus
    {
        Invalid,
        OutOfRange,
        TooSmall,
        TooLarge,
        Rerolled
    };

    struct Round
    {
        int targetNumber;
        int wrongAttempts = 0;
        Clock::time_point startTime = Clock::now();
    };

    GameplayOptions options;
    Round round;

    std::string statusMessage;

    std::chrono::seconds getElapsedTime() const;

    int generateRandomNumber();
    std::string_view selectRandomGreaterNumberPhrase();
    std::string_view selectRandomSmallerNumberPhrase();

    std::string selectStatusMessage(GuessStatus status);
    std::string constructAttemptsIndicatorText() const;
    std::string constructAttemptsUntilRerollText() const;

    FrameTransition finish(GameplayOutcome outcome) const;

    bool isNewGamePlusMode() const;
    bool isChallengeMode() const;
    int getMaxAttempts() const;
    int getRerollInterval() const;
    int getAttemptsUntilReroll() const;

    void reroll();

public:
    GameplayState(GameContext& context, GameplayOptions options);

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};

