#pragma once

#include <concepts>
#include <format>
#include <string>
#include <string_view>
#include <utility>
#include <tuple>


struct Settings;

class Translator
{
private:
    struct NonTranslatableArg
    {
        std::string_view text;
    };

    const Settings& settings;

    std::string_view prepareFormatArg(NonTranslatableArg arg) const { return arg.text; }

    template <typename T>
    auto prepareFormatArg(T&& arg) const
    {
        if constexpr (std::convertible_to<T, std::string_view>)
            return this->translateView(std::string_view{ std::forward<T>(arg) });

        else
            return std::forward<T>(arg);
    }

    template <typename Tuple, std::size_t... I>
    std::string formatPrepared(
        std::string_view text,
        Tuple& args,
        std::index_sequence<I...>        
    ) const
    {
        return std::vformat(
            this->translateView(text),
            std::make_format_args(std::get<I>(args)...)
        );
    }

public:
    Translator(const Settings& settings) : settings(settings) {}

    std::string_view translateView(std::string_view text) const;
    std::string translateString(std::string_view text) const;

    NonTranslatableArg notTranslate(std::string_view text) const { return NonTranslatableArg{ text }; }

    template <typename... Args>
    std::string format(std::string_view text, Args&&... args) const
    {
        auto preparedArgs = std::tuple{ this->prepareFormatArg(std::forward<Args>(args))... };

        return this->formatPrepared(text, preparedArgs, std::index_sequence_for<Args...>{});
    }
};
