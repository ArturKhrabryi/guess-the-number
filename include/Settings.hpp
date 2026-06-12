#pragma once

#include <string_view>
#include <stdexcept>


enum class Language
{
    English,
    Polish
};

[[nodiscard]] constexpr std::string_view toString(Language language)
{
    using enum Language;

    switch (language)
    {
    case English:
        return "English";

    case Polish:
        return "Polish";
    }

    throw std::invalid_argument("invalid Language value");
}

struct Settings
{
    Language language = Language::English;
    bool askChallengeMode = true;
};
