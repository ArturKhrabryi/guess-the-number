#include "Renderer.hpp"

#include "Screen.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace
{
#ifdef _WIN32
    void enableVirtualTerminal(HANDLE handle)
    {
        if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
            return;

        DWORD mode = 0;
        if (!GetConsoleMode(handle, &mode))
            return;

        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handle, mode);
    }

    void initConsole()
    {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        enableVirtualTerminal(GetStdHandle(STD_OUTPUT_HANDLE));
    }
#endif
}

Renderer::Renderer()
{
#ifdef _WIN32
    initConsole();
#endif

    enterAlternateScreen();
    clear();
}

Renderer::~Renderer()
{
    clear();
    leaveAlternateScreen();
}

void Renderer::render(const Screen& screen) const
{
    clear();

    screen.visitElements([this](const auto& element) {
            this->renderElement(element);
    });

    std::cout << "\n> " << std::flush;
} 

void Renderer::clear()
{
    std::cout << "\x1b[2J\x1b[H";
}

void Renderer::enterAlternateScreen()
{
    std::cout << "\x1b[?1049h" << std::flush;
}

void Renderer::leaveAlternateScreen()
{
    std::cout << "\x1b[?1049l" << std::flush;
}

void Renderer::renderElement(const TextElement& element)
{
    switch (element.role)
    {
    case TextRole::Title:
        std::cout << element.text << "\n\n";
        break;

    case TextRole::Footer:
        std::cout << '\n' << element.text << '\n';
        break;

    case TextRole::Error:
        std::cout << "[Error] " << element.text << '\n';
        break;

    case TextRole::Normal:
        std::cout << element.text << '\n';
        break;

    case TextRole::Debug:
        std::cout << "[Debug] " << element.text << '\n';
    }
}

void Renderer::renderElement(const MenuElement& element)
{
    for (std::size_t i = 0; i < element.items.size(); ++i)
        std::cout << i + 1 << ": " << element.items[i] << '\n';
}

void Renderer::renderElement(const SpacerElement& element)
{
    for (std::size_t i = 0; i < element.lines; ++i)
        std::cout << '\n';
}
