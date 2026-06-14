#include "StateFrame.hpp"

#include "StateInterfaces.hpp"

#include <stdexcept>
#include <memory>
#include <utility>


StateFrame::StateFrame(std::unique_ptr<GameState> state) :
    state(std::move(state))
{
    if (!this->state)
        throw std::logic_error("StateFrame contains null state");

    auto statePtr = this->state.get();

    this->screenState = dynamic_cast<ScreenProvider*>(statePtr);
    this->enterHandler = dynamic_cast<EnterHandler*>(statePtr);
    this->returnHandler = dynamic_cast<ReturnHandler*>(statePtr);
}
