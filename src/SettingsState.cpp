#include "SettingsState.hpp"
#include "GameContext.hpp"
#include "HallOfFameScores.hpp"
#include "Screen.hpp"
#include "Settings.hpp"

#include <format>
#include <stdexcept>


HallOfFameClearConfirmationStep::HallOfFameClearConfirmationStep(GameContext& context) :
    GameState(context)
{
    this->addMenuItem(MenuItem{
        .text = "No",
        .handler = [] {
            return HallOfFameClearConfirmationStep::makeReturn(false);
        }        
    });

    this->addMenuItem(MenuItem{
        .text = "Yes",
        .handler = [] {
            return HallOfFameClearConfirmationStep::makeReturn(true);
        }        
    });
}

Screen HallOfFameClearConfirmationStep::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
        .text = "Have you though about it? Do you want to remove all the scores?"
    });

    return screen;
}

void SettingsState::deleteHallOfFame()
{
    this->getContext().hallOfFameScores.clear();
}

void SettingsState::insertHeader(Screen& screen) const
{
    if (!this->statusMessage.empty())
    {
        screen.header.push_back(TextElement{ .text = this->statusMessage });
        screen.header.push_back(SpacerElement{});
    }

    Language language = this->getContext().settings.language;
    screen.header.push_back(TextElement{ std::format("Current language: {}", toString(language)) });

    bool askChallengeMode = this->getContext().settings.askChallengeMode;
    screen.header.push_back(TextElement{ std::format("The game {} about the challenge mode", askChallengeMode ? "asks" : "doesn't ask") });
}

SettingsState::SettingsState(GameContext& context) : GameState(context)
{
    this->addMenuItem(MenuItem{
        .text = "Clear hall of fame",
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<HallOfFameClearConfirmationStep>() };
        }
    });

    this->addMenuItem(MenuItem{
        .text = "Toggle challenge mode question",
        .handler = [this] {
            bool& currentAskChallengeMode = this->getContext().settings.askChallengeMode;
            currentAskChallengeMode = !currentAskChallengeMode;

            this->statusMessage = "Challenge mode asking was changed";

            return NoneTransition{};
        }
    });

    this->addMenuItem(MenuItem{
        .text = "Return to main menu",
        .handler = [] { return ReturnTransition{}; }
    });
}

FrameTransition SettingsState::handleReturn(std::any value)
{
    this->statusMessage.clear();

    if (auto* definitelyDelete = std::any_cast<HallOfFameClearConfirmationStep::ReturnType>(&value))
    {
        if (*definitelyDelete)
        {
            this->deleteHallOfFame();

            this->statusMessage = "Hall of fame was cleared";
        }

        return NoneTransition{};
    }

    throw std::logic_error("Unknown return type");
}

Screen SettingsState::getScreen() const
{
    auto screen = MenuScreenState::getScreen();
    
    this->insertHeader(screen);

    return screen;
}
