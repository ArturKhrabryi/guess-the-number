#pragma once


struct Screen;
struct TextElement;
struct MenuElement;
struct SpacerElement;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void render(const Screen& screen) const;

private:
    static void clear();
    static void enterAlternateScreen();
    static void leaveAlternateScreen();

    static void renderElement(const TextElement& element);
    static void renderElement(const MenuElement& element);
    static void renderElement(const SpacerElement& element);
};
