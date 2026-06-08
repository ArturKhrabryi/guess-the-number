#pragma once

#include <charconv>
#include <optional>
#include <string>
#include <iostream>
#include <string_view>
#include <variant>
#include <concepts>
#include <system_error>
#include <algorithm>
#include <cctype>


struct Event
{
    std::string value; 
};

struct EndOfInput {};

using InputResult = std::variant<Event, EndOfInput>;

class InputHandler
{
private:
    static void trim(std::string& str)
    {
        auto isNotSpace = [](unsigned char ch) { return !std::isspace(ch); };
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), isNotSpace));
        str.erase(std::find_if(str.rbegin(), str.rend(), isNotSpace).base(), str.end());
    }

public:
    static InputResult readEvent()
    { 
        std::string line;
        if (!std::getline(std::cin, line))
            return EndOfInput{};

        trim(line);

        return Event{ .value = std::move(line) };
    }

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
