#include "MenuScreenState.hpp"

#include "Screen.hpp"
#include "InputHandler.hpp"

#include <vector>
#include <string>
#include <cstddef>
#include <utility>


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
