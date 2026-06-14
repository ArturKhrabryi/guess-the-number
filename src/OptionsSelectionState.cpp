#include "OptionsSelectionState.hpp"
#include "GameContext.hpp"
#include "GameplayDifficulty.hpp"
#include "InputHandler.hpp"
#include "Screen.hpp"
#include "Settings.hpp"
#include "Translator.hpp"

#include <cstddef>
#include <stdexcept>


std::string DifficultySelectionStep::getTitle() const
{
    return this->getContext().translator.translateString("Difficulty selection");
}

DifficultySelectionStep::DifficultySelectionStep(GameContext& context) : MenuScreenState(context)
{
    for (std::size_t i = 0; i < getDifficultyCount(); ++i)
    {
        const GameplayDifficulty difficulty = difficultyFromIndex(i);

        this->addMenuItem(MenuItem{
            .textProvider = [this, difficulty] {
                const auto limits = getRandomNumberLimits(difficulty);

                return this->getContext().translator.format("{} ({}-{})", toString(difficulty), limits.min, limits.max);
            },
            .handler = [difficulty] {
                return DifficultySelectionStep::makeReturn(difficulty);
            }
        });
    }
}

Screen DifficultySelectionStep::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
        .text = this->getContext().translator.translateString("What difficulty would you like?")
    });

    return screen;
}

std::string GameplayModeSelectionStep::getTitle() const
{
    return this->getContext().translator.translateString("Game mode selection");
}

GameplayModeSelectionStep::GameplayModeSelectionStep(GameContext& context) : MenuScreenState(context)
{
    const auto& translator = this->getContext().translator;

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Standard mode");
        },
        .handler = [] {
            return GameplayModeSelectionStep::makeReturn(GameplayMode{ StandardMode{} });
        }
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("New game plus");
        },
        .handler = [] {
            return GameplayModeSelectionStep::makeReturn(GameplayMode{ NewGamePlusMode{} });
        }
    });
}

Screen GameplayModeSelectionStep::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
        .text = this->getContext().translator.translateString("Which game mode do you want to play?")
    });

    return screen;
}

std::string ChallengeModeSelectionStep::getTitle() const
{
    return this->getContext().translator.translateString("Enabling challenge mode");
}

ChallengeModeSelectionStep::ChallengeModeSelectionStep(GameContext& context) : MenuScreenState(context)
{
    const auto& translator = this->getContext().translator;

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("No");
        },
        .handler = [] {
            return ChallengeModeSelectionStep::makeReturn(false);
        }
    });

    this->addMenuItem(MenuItem{
        .textProvider = [&translator] {
            return translator.translateString("Yes");
        },
        .handler = [] {
            return ChallengeModeSelectionStep::makeReturn(true);
        }
    });
}

Screen ChallengeModeSelectionStep::getScreen() const
{
    auto screen = MenuScreenState::getScreen();

    screen.header.push_back(TextElement{
        .text = this->getContext().translator.translateString("Would you like to enable challenge mode?")
    });

    return screen;
}

Screen MaxAttemptsSelectionStep::getScreen() const
{
    const auto& translator = this->getContext().translator;

    Screen screen{
        translator.translateString("Attempts limit selection"),
        translator.translateString("Attempts limit:")
    };

    screen.header.push_back(TextElement{
        .text = translator.translateString("How many tries do you think it will take you to guess?")
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
        {
            bool askChallengeMode = this->getContext().settings.askChallengeMode;
            if (askChallengeMode)
                return PushStateTransition{ .nextState = this->makeState<ChallengeModeSelectionStep>() };

            return OptionsSelectionState::makeReturn(this->options);
        }

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
