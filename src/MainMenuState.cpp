#include "MainMenuState.hpp"
#include "GameState.hpp"
#include "GameTypes.hpp"
#include "OptionsSelectionState.hpp"
#include "PostGameState.hpp"
#include "HallOfFameState.hpp"
#include "GameplayState.hpp"
#include "SettingsState.hpp"

#include <any>
#include <stdexcept>


MainMenuState::MainMenuState(GameContext& context) : GameState(context)
{
    this->addMenuItem(MenuItem{
        .text = "Start new game",
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<OptionsSelectionState>() };
        }
    });

    this->addMenuItem(MenuItem{
        .text = "Hall of fame",
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<HallOfFameState>() };
        },
        .isVisible = [this] {
            return !this->getContext().hallOfFameScores.isEmpty();
        }
    });

    this->addMenuItem(MenuItem{
        .text = "Settings",
        .handler = [this] {
            return PushStateTransition{ .nextState = this->makeState<SettingsState>() };
        }
    });

    this->addMenuItem(MenuItem{
        .text = "Quit the game",
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
