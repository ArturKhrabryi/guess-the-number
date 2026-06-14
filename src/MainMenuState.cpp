#include "MainMenuState.hpp"
#include "GameContext.hpp"
#include "HallOfFameScores.hpp"
#include "OptionsSelectionState.hpp"
#include "PostGameState.hpp"
#include "HallOfFameState.hpp"
#include "GameplayState.hpp"
#include "SettingsState.hpp"
#include "Translator.hpp"

#include <any>
#include <stdexcept>
#include <string>


std::string MainMenuState::getTitle() const
{
    return this->getContext().translator.translateString("Main menu");
} 

MainMenuState::MainMenuState(GameContext& context) : MenuScreenState(context)
{
    const auto& translator = this->getContext().translator;

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Start new game");
        },
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<OptionsSelectionState>() };
        }
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Hall of fame");
        },
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<HallOfFameState>() };
        },
        .isVisible = [this] {
            return !this->getContext().hallOfFameScores.isEmpty();
        }
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Settings");
        },
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<SettingsState>() };
        }
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Quit the game");
        },
        .handler = [] {
            return QuitGameTransition{};
        }
    });
}

FrameTransition MainMenuState::handleReturn(std::any value)
{
    if (ReturnHandler::isVoid(value))
        return NoneTransition{};

    if (auto* gameplayOptions = std::any_cast<OptionsSelectionState::ReturnType>(&value))
        return PushStateTransition{ .nextState = this->makeState<GameplayState>(*gameplayOptions) };

    if (auto* gameplayResult = std::any_cast<GameplayState::ReturnType>(&value))
        return PushStateTransition{ .nextState = this->makeState<PostGameState>(*gameplayResult) };

    throw std::logic_error("Unknown return type");
}
