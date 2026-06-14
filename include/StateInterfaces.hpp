#pragma once

#include "FrameTransition.hpp"

#include <any>
#include <concepts>
#include <type_traits>
#include <utility>


struct Screen;
struct Event;

class ScreenProvider
{
public:
    virtual ~ScreenProvider() = default;
    virtual Screen getScreen() const = 0;
    virtual FrameTransition handleEvent(const Event& event) = 0;
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
