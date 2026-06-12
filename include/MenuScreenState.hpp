#pragma once

#include "StateInterfaces.hpp"
#include "FrameTransition.hpp"

#include <string>
#include <string_view>
#include <functional>
#include <vector>
#include <utility>


struct Screen;
struct Event;

class MenuScreenState : public ScreenState
{
protected:
    struct MenuItem
    {
        std::string text;
        std::function<FrameTransition()> handler;
        std::function<bool()> isVisible = [] {
            return true;
        };
    };

private:
    std::vector<MenuItem> menuItems;

protected:
    void addMenuItem(MenuItem item) { this->menuItems.push_back(std::move(item)); }

    virtual std::string_view getTitle() const = 0;
    virtual std::string_view getFooter() const { return "Choose an option"; }

public:
    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};
