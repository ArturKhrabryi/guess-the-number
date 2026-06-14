#include "SettingsState.hpp"
#include "FrameTransition.hpp"
#include "GameContext.hpp"
#include "HallOfFameScores.hpp"
#include "Screen.hpp"
#include "Settings.hpp"
#include "Translator.hpp"

#include <stdexcept>
#include <string>

std::string HallOfFameClearConfirmationStep::getTitle() const
{
    return this->getContext().translator.translateString("Hall of fame clear confirmation");
}

HallOfFameClearConfirmationStep::HallOfFameClearConfirmationStep(GameContext& context) :
    MenuScreenState(context)
{
    const auto& translator = this->getContext().translator;

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("No");
        },
        .handler = [] {
            return HallOfFameClearConfirmationStep::makeReturn(false);
        }        
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Yes");
        },
        .handler = [] {
            return HallOfFameClearConfirmationStep::makeReturn(true);
        }        
    });
}

Screen HallOfFameClearConfirmationStep::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
        .text = this->getContext().translator.translateString("Have you thought about it? Do you want to remove all the scores?")
    });

    return screen;
}

std::string LanguageSelectionState::getTitle() const
{
    return this->getContext().translator.translateString("Language selection");
}

LanguageSelectionState::LanguageSelectionState(GameContext& context) :
    MenuScreenState(context)
{
    using enum Language;

    const auto& translator = this->getContext().translator; 

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Polish");
        },
        .handler = [] {
            return LanguageSelectionState::makeReturn(Polish);
        }        
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("English");
        },
        .handler = [] {
            return LanguageSelectionState::makeReturn(English);
        }        
    });
}

Screen LanguageSelectionState::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
            .text = this->getContext().translator.translateString("Select the language")
    });

    return screen;
}

void SettingsState::clearHallOfFame()
{
    this->getContext().hallOfFameScores.clear();
}

void SettingsState::insertHeader(Screen& screen) const
{
    if (!this->statusMessage.empty())
    {
        screen.header.push_back(TextElement{
            .text = this->statusMessage
        });
        screen.header.push_back(SpacerElement{});
    }

    const auto& translator = this->getContext().translator;
    
    Language language = this->getContext().settings.language;
    screen.header.push_back(TextElement{
        .text = translator.format("Current language: {}", toString(language))
    });

    bool askChallengeMode = this->getContext().settings.askChallengeMode;
    screen.header.push_back(TextElement{
        .text = askChallengeMode ?
            translator.translateString("The game asks about the challenge mode") :
            translator.translateString("The game doesn't ask about the challenge mode")
    });
}

std::string SettingsState::getTitle() const
{
    return this->getContext().translator.translateString("Settings");
}

SettingsState::SettingsState(GameContext& context) : MenuScreenState(context)
{
    const auto& translator = this->getContext().translator;

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Change language");
        },
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<LanguageSelectionState>() };
        }
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Clear hall of fame");
        },
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<HallOfFameClearConfirmationStep>() };
        }
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Toggle challenge mode question");
        },
        .handler = [this] {
            bool& currentAskChallengeMode = this->getContext().settings.askChallengeMode;
            currentAskChallengeMode = !currentAskChallengeMode;

            this->statusMessage = this->getContext().translator.translateString("Challenge mode asking was changed");

            return NoneTransition{};
        }
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Return to main menu");
        },
        .handler = [] { return ReturnTransition{}; }
    });
}

FrameTransition SettingsState::handleReturn(std::any value)
{
    const auto& translator = this->getContext().translator;

    this->statusMessage.clear();

    if (auto* language = std::any_cast<LanguageSelectionState::ReturnType>(&value))
    {
        if (this->getContext().settings.language == *language)
            this->statusMessage = translator.translateString("You picked the same language. Well done");

        else
        {
            this->getContext().settings.language = *language;

            this->statusMessage = translator.translateString("Language has been changed");
        }

        return NoneTransition{};
    }

    if (auto* definitelyDelete = std::any_cast<HallOfFameClearConfirmationStep::ReturnType>(&value))
    {
        if (*definitelyDelete)
        {
            this->clearHallOfFame();

            this->statusMessage = translator.translateString("Hall of fame was cleared");
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
