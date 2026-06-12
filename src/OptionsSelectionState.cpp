#include "OptionsSelectionState.hpp"
#include "GameState.hpp"
#include "GameTypes.hpp"
#include "InputHandler.hpp"
#include "Screen.hpp"

#include <cstddef>
#include <format>
#include <memory>
#include <stdexcept>


auto DifficultySelectionStep::constructDifficultyItem(GameplayDifficulty difficulty) -> DifficultyItem
{
    const auto name = toString(difficulty);
    const auto limits = getRandomNumberLimits(difficulty);

    return {
        .difficulty = difficulty,
        .name = std::format("{} ({}-{})", name, limits.min, limits.max)
    };
}

auto DifficultySelectionStep::constructDifficultyItems() -> std::vector<DifficultyItem>
{
    std::vector<DifficultyItem> res;
    constexpr auto size = getDifficultyCount();
    res.reserve(size);
    for (std::size_t i = 0; i < size; ++i)
    {
        GameplayDifficulty difficulty = difficultyFromIndex(i);
        res.push_back(constructDifficultyItem(difficulty));
    }

    return res;
}

DifficultySelectionStep::DifficultySelectionStep(GameContext& context) : GameState(context)
{
    auto difficultyItems = this->constructDifficultyItems();
    for (auto& difficultyItem : difficultyItems)
    {
        this->addMenuItem(MenuItem{
            .text = std::move(difficultyItem.name),
            .handler = [difficulty = difficultyItem.difficulty] {
                return DifficultySelectionStep::makeReturn(difficulty);
            }
        });
    }
}

Screen DifficultySelectionStep::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
        .text = "What difficulty would you like?"
    });

    return screen;
}

GameplayModeSelectionStep::GameplayModeSelectionStep(GameContext& context) : GameState(context)
{
    this->addMenuItem(MenuItem{
        .text = "Standard mode",
        .handler = [] {
            return GameplayModeSelectionStep::makeReturn(GameplayMode{ StandardMode{} });
        }
    });

    this->addMenuItem(MenuItem{
        .text = "New game plus",
        .handler = [] {
            return GameplayModeSelectionStep::makeReturn(GameplayMode{ NewGamePlusMode{} });
        }
    });
}

Screen GameplayModeSelectionStep::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
        .text = "Which game mode do you want to play?"
    });

    return screen;
}

ChallengeModeSelectionStep::ChallengeModeSelectionStep(GameContext& context) : GameState(context)
{
    this->addMenuItem(MenuItem{
        .text = "No",
        .handler = [] {
            return ChallengeModeSelectionStep::makeReturn(false);
        }
    });

    this->addMenuItem(MenuItem{
        .text = "Yes",
        .handler = [] {
            return ChallengeModeSelectionStep::makeReturn(true);
        }
    });
}

Screen ChallengeModeSelectionStep::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
        .text = "Would you like to enable challenge mode?"
    });

    return screen;
}

Screen MaxAttemptsSelectionStep::getScreen() const
{
    Screen screen{
        "Attempts limit selection",
        "Attempts limit:"
    };

    screen.header.push_back(TextElement{
        .text = "How many tries do you think it will take you to guess?"
    });

    return screen;
}

FrameTransition MaxAttemptsSelectionStep::handleEvent(const Event& event)
{
    auto maxAttemptsOptional = InputHandler::toNumber<int>(event.value);
    if (!maxAttemptsOptional)
        return NoneTransition{};

    auto maxAttempts = *maxAttemptsOptional;

    if (maxAttempts <= 0)
        return NoneTransition{};

    return MaxAttemptsSelectionStep::makeReturn(maxAttempts);
}

FrameTransition OptionsSelectionState::onEnter()
{
    return PushStateTransition{ .nextState = this->makeState<DifficultySelectionStep>() };
}

FrameTransition OptionsSelectionState::handleReturn(std::any value)
{
    if (auto* difficulty = std::any_cast<DifficultySelectionStep::ReturnType>(&value))
    {
        this->options.difficulty = *difficulty; 
        
        return PushStateTransition{ .nextState = this->makeState<GameplayModeSelectionStep>() };
    }

    if (auto* gameMode = std::any_cast<GameplayModeSelectionStep::ReturnType>(&value))
    {
        this->options.mode = *gameMode;
        
        if (std::holds_alternative<StandardMode>(*gameMode))
            return PushStateTransition{ .nextState = this->makeState<ChallengeModeSelectionStep>() };

        return OptionsSelectionState::makeReturn(this->options);
    }

    if (auto* isChallengeMode = std::any_cast<ChallengeModeSelectionStep::ReturnType>(&value))
    {
        if (*isChallengeMode)
            return PushStateTransition{ .nextState = this->makeState<MaxAttemptsSelectionStep>() };

        return OptionsSelectionState::makeReturn(this->options);
    }

    if (auto* maxAttempts = std::any_cast<MaxAttemptsSelectionStep::ReturnType>(&value))
    {
        std::get<StandardMode>(this->options.mode).maxAttempts = *maxAttempts;

        return OptionsSelectionState::makeReturn(this->options);
    }

    throw std::logic_error("Unknown return type");
}
