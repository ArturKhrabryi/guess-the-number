#include "PostGameState.hpp"
#include "GameState.hpp"
#include "Screen.hpp"


Screen PostGameState::getScreen() const
{
    auto title = std::string { toString(this->result.outcome) };
    std::string footer;
    TextElement attemptsMessageBox;

    bool isVictory = this->result.outcome == GameplayOutcome::Victory;
    if (isVictory)
    {
        if (this->result.wrongAttempts == 0)
            attemptsMessageBox.text = "Wow! You guessed the number on your first try";

        else if (this->result.wrongAttempts == 1)
            attemptsMessageBox.text = "You made a mistake just once";

        else
            attemptsMessageBox.text = "You made " + std::to_string(this->result.wrongAttempts) + " mistakes in total";

        footer = "Enter your name. You will return to the main menu";
    }
    else
    {
        attemptsMessageBox.text = "You ran out of attempts";
        footer = "Enter any input to return to the main menu";
    }

    Screen screen(std::move(title), std::move(footer));
    screen.header.push_back(std::move(attemptsMessageBox));

    return screen;
}

FrameTransition PostGameState::handleEvent(const Event& event)
{
    if (this->result.outcome == GameplayOutcome::Defeat)
        return PopStateTransition{};

    if (event.value.empty())
        return NoneTransition{};

    ReturnType gameScoreAndDifficulty = { .score = { .name = event.value, .attempts = this->result.wrongAttempts + 1 }, .difficulty = this->result.difficulty };

    return ReturnTransition{ .value = std::move(gameScoreAndDifficulty) };
}
