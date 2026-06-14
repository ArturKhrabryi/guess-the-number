#include "PostGameState.hpp"
#include "Event.hpp"
#include "GameContext.hpp"
#include "HallOfFameScores.hpp"
#include "Screen.hpp"
#include "Translator.hpp"

#include <variant>


Screen PostGameState::getScreen() const
{
    const auto& translator = this->getContext().translator;

    auto title = translator.translateString(toString(this->result.outcome));
    std::string footer;
    TextElement attemptsMessageBox;

    bool isVictory = this->result.outcome == GameplayOutcome::Victory;
    if (isVictory)
    {
        if (this->result.wrongAttempts == 0)
            attemptsMessageBox.text = translator.translateString("Wow! You guessed the number on your first try");

        else if (this->result.wrongAttempts == 1)
            attemptsMessageBox.text = translator.translateString("You made a mistake just once");

        else
            attemptsMessageBox.text = translator.format("You made {} mistakes in total", this->result.wrongAttempts);

        footer = translator.translateString("Enter your name. You will return to the main menu");
    }
    else
    {
        attemptsMessageBox.text = translator.translateString("You ran out of attempts");
        footer = translator.translateString("Enter any input to return to the main menu");
    }

    Screen screen(std::move(title), std::move(footer));
    screen.header.push_back(std::move(attemptsMessageBox));
    screen.header.push_back(TextElement{ .text = translator.format("The game took {}", this->result.gameDuration) });

    return screen;
}

FrameTransition PostGameState::handleEvent(const Event& event)
{
    if (this->result.outcome == GameplayOutcome::Defeat)
        return ReturnTransition{};

    if (event.value.empty())
        return NoneTransition{};

    GameScore score{
        .name = event.value,
        .attempts = this->result.wrongAttempts + 1,
        .gameDuration = this->result.gameDuration,
        .isNewGamePlus = std::holds_alternative<NewGamePlusMode>(this->result.mode)
    };

    this->getContext().hallOfFameScores.addGameScore(std::move(score), this->result.difficulty);

    return ReturnTransition{};
}
