#include "Game.hpp"
#include "MainMenuState.hpp"

#include <exception>
#include <iostream>


int main()
{
    try
    {
        Game game;

        game.pushState<MainMenuState>();

        game.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Unhandled exception: " << ex.what() << std::endl;

        return 1;
    }

    return 0;
}
