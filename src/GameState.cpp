#include "GameState.hpp"
#include "InputHandler.hpp"
#include "Screen.hpp"

#include <stdexcept>


StateFrame::StateFrame(std::unique_ptr<GameState> state) :
    state(std::move(state))
{
    if (!this->state)
        throw std::logic_error("StateFrame contains null state");

    auto statePtr = this->state.get();

    this->screenState = dynamic_cast<ScreenState*>(statePtr);
    this->enterHandler = dynamic_cast<EnterHandler*>(statePtr);
    this->returnHandler = dynamic_cast<ReturnHandler*>(statePtr);
}

Screen MenuScreenState::getScreen() const
{
    Screen screen{
        std::string{ this->getTitle() },
        std::string{ this->getFooter() }
    };

    std::vector<std::string> textItems;
    for (const auto& item : this->menuItems)
    {
        if (item.isVisible())
            textItems.push_back(item.text);
    }

    screen.body.push_back(MenuElement{ .items = std::move(textItems) });

    return screen;
}

FrameTransition MenuScreenState::handleEvent(const Event& event)
{
    auto selectedOption = InputHandler::toNumber<std::size_t>(event.value);
    if (!selectedOption)
        return NoneTransition{};

    const auto optionNumber = *selectedOption;

    if (optionNumber == 0)
        return NoneTransition{};

    std::size_t visibleIndex = 0;

    for (const auto& item : this->menuItems)
    {
        if (!item.isVisible())
            continue;

        ++visibleIndex;

        if (visibleIndex == optionNumber)
            return item.handler();
    }

    return NoneTransition{};
}
