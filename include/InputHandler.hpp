#pragma once

#include <charconv>
#include <optional>
#include <string_view>
#include <string>
#include <concepts>
#include <system_error>

#include "Event.hpp"


class InputHandler
{
private:
    static void trim(std::string& str);

public:
    static InputResult readEvent();

    template <typename T>
    requires (std::integral<T> || std::floating_point<T>)
    static std::optional<T> toNumber(std::string_view str)
    {
        T value{};
        const char* begin = str.data();
        const char* end = str.data() + str.size();

        auto [ptr, ec] = std::from_chars(begin, end, value);
        if (ec != std::errc{} || ptr != end)
            return std::nullopt;

        return value;
    }
};
