#include "MenuScreenState.hpp"

#include "GameContext.hpp"
#include "Screen.hpp"
#include "InputHandler.hpp"
#include "Translator.hpp"

#include <vector>
#include <string>
#include <cstddef>
#include <utility>


std::string MenuScreenState::getFooter() const
{
    return this->getContext().translator.translateString("Choose an option");
}

Screen MenuScreenState::getScreen() const
{
    Screen screen{
        this->getTitle(),
        this->getFooter()
    };

    std::vector<std::string> menuTextElements;
    for (const auto& item : this->menuItems)
    {
        if (item.isVisible())
            menuTextElements.push_back(item.textProvider());
    }

    screen.body.push_back(MenuElement{ .items = std::move(menuTextElements) });

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
