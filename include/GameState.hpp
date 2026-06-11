#pragma once

#include <any>
#include <concepts>
#include <functional>
#include <memory>
#include <variant>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <utility>

#include "GameTypes.hpp"
#include "Screen.hpp"
#include "InputHandler.hpp"


class GameState;

struct NoneTransition {};
struct QuitGameTransition {};

struct ChangeStateTransition
{
    std::unique_ptr<GameState> nextState{}; 
};

struct PushStateTransition
{
    std::unique_ptr<GameState> nextState{}; 
};

struct VoidReturn{};

struct ReturnTransition
{
    std::any value = VoidReturn{};
};

using FrameTransition = std::variant<
    NoneTransition,
    QuitGameTransition,
    ChangeStateTransition,
    PushStateTransition,
    ReturnTransition
>;

class GameState
{
private:
    GameContext& context;

protected:
    GameContext& getContext() { return this->context; }
    const GameContext& getContext() const { return this->context; }

    template <typename State, typename... Args>
    std::unique_ptr<State> makeState(Args&&... args)
    {
        return std::make_unique<State>(this->context, std::forward<Args>(args)...);
    }

public:
    virtual ~GameState() = default;

    explicit GameState(GameContext& context) : context(context) {}
};

class ScreenState
{
public:
    virtual ~ScreenState() = default;
    virtual Screen getScreen() const = 0;
    virtual FrameTransition handleEvent(const Event& event) = 0;
};

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

class EnterHandler
{
public:
    virtual ~EnterHandler() = default;
    virtual FrameTransition onEnter() = 0;
};

class ReturnHandler
{
protected:
    static bool isVoid(const std::any& value) noexcept;

public:
    virtual ~ReturnHandler() = default;
    virtual FrameTransition handleReturn(std::any value) = 0;
};

template <typename T>
class Returns
{
public:
    using ReturnType = T;

protected:
    template <typename U>
    static ReturnTransition makeReturn(U&& value)
    requires std::same_as<std::remove_cvref_t<U>, ReturnType>
    {
        return ReturnTransition{
            .value = std::forward<U>(value)
        };
    }
};

struct StateFrame
{
private:
    std::unique_ptr<GameState> state;

public:
    explicit StateFrame(std::unique_ptr<GameState> state);

    StateFrame(StateFrame&&) = default;
    StateFrame& operator=(StateFrame&&) = default;

    ScreenState* screenState = nullptr;
    EnterHandler* enterHandler = nullptr;
    ReturnHandler* returnHandler = nullptr;

};
