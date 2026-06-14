#pragma once

#include "GameState.hpp"
#include "StateInterfaces.hpp"
#include "FrameTransition.hpp"

#include <string>
#include <functional>
#include <vector>
#include <utility>


struct Screen;
struct Event;

class MenuScreenState : public GameState, public ScreenProvider
{
protected:
    struct MenuItem
    {
        std::function<std::string()> textProvider;
        std::function<FrameTransition()> handler;
        std::function<bool()> isVisible = [] {
            return true;
        };
    };

private:
    std::vector<MenuItem> menuItems;

protected:
    void addMenuItem(MenuItem item) { this->menuItems.push_back(std::move(item)); }

    virtual std::string getTitle() const = 0;
    virtual std::string getFooter() const;

public:
    using GameState::GameState;

    virtual Screen getScreen() const override;
    virtual FrameTransition handleEvent(const Event& event) override;
};
