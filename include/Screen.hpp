#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include <variant>
#include <utility>


enum class TextRole
{
    Title,
    Normal,
    Footer,
    Error,
    Debug
};

struct TextElement
{
    std::string text;
    TextRole role{ TextRole::Normal };
};

struct MenuElement
{
    std::vector<std::string> items;
};

struct SpacerElement
{
    std::size_t lines{ 1 };
};

using ScreenElement = std::variant<
    TextElement,
    MenuElement,
    SpacerElement
>;

struct Screen
{
    TextElement title;
    std::vector<ScreenElement> header{};
    std::vector<ScreenElement> body{};
    TextElement footer;

    Screen(std::string title, std::string footer) :
        title{
            .text = std::move(title),
            .role = TextRole::Title
        },
        footer{
            .text = std::move(footer),
            .role = TextRole::Footer
        }
    {
    } 

    template <typename Visitor>
    void visitElements(Visitor&& visitor) const
    {
        std::invoke(visitor, this->title);

        for (const auto& element : this->header)
            std::visit(visitor, element);

        if (!this->header.empty() && !this->body.empty())
            std::invoke(visitor, SpacerElement{});

        for (const auto& element : this->body)
            std::visit(visitor, element);

        std::invoke(visitor, this->footer);
    }
};
