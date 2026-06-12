#include "Game.hpp"

#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>

#include "InputHandler.hpp"
#include "Screen.hpp"
#include "StateInterfaces.hpp"


void Game::enterCurrentFrame()
{
    if (this->frames.empty())
        return;

    auto& currentFrame = this->frames.back();
    if (!currentFrame.enterHandler)
        return;

    FrameTransition enterTransition = currentFrame.enterHandler->onEnter();
    
    this->processTransition(std::move(enterTransition));
}

void Game::processTransition(FrameTransition transition)
{
    std::visit([this](auto&& tr) { this->applyTransition(std::move(tr)); }, std::move(transition));
}

void Game::applyTransition(QuitGameTransition)
{
    this->frames.clear();
}

void Game::applyTransition(ChangeStateTransition tr)
{
    if (!tr.nextState)
        throw std::logic_error("ChangeStateTransition contains null nextState");

    if (this->frames.empty())
        this->frames.emplace_back(std::move(tr.nextState));

    else
        this->frames.back() = StateFrame{ std::move(tr.nextState) };

    this->enterCurrentFrame();
}

void Game::applyTransition(PushStateTransition tr)
{
    if (!tr.nextState)
        throw std::logic_error("PushStateTransition contains null nextState");

    this->frames.emplace_back(std::move(tr.nextState));
    this->enterCurrentFrame();
}

void Game::applyTransition(ReturnTransition tr)
{
    if (this->frames.empty())
        return;

    this->frames.pop_back();
    
    if (this->frames.empty())
        return;
    
    auto* parent = this->frames.back().returnHandler;
    bool isVoid = std::any_cast<VoidReturn>(&tr.value) != nullptr;

    if (!parent)
    {
        if (!isVoid)
            throw std::logic_error("Parent cannot handle return value");

        return;
    }

    FrameTransition nextTransition = parent->handleReturn(std::move(tr.value));
    
    this->processTransition(std::move(nextTransition));
}

void Game::run()
{
    while (!this->frames.empty())
    {
        auto& currentFrame = this->frames.back();
        if (!currentFrame.screenState)
            throw std::logic_error("Top state is not renderable");

        {
            Screen screen = currentFrame.screenState->getScreen();
            this->renderer.render(screen);
        }

        auto inputResult = InputHandler::readEvent();
        if (const auto* event = std::get_if<Event>(&inputResult))
        {
            FrameTransition transition = currentFrame.screenState->handleEvent(*event);
            this->processTransition(std::move(transition));

            continue;
        }

        if (std::holds_alternative<EndOfInput>(inputResult))
            this->frames.clear();
    }
}
