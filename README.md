# Guess the Number

`Guess the Number` is a terminal-based C++20 sequel to the classic "guess the hidden number" assignment. The game keeps the simple rules, but adds a state-driven menu system, Hall of Fame rankings, settings, Polish/English localization, challenge mode, New Game Plus, game duration tracking, and a cleaner full-screen terminal experience.

The project is intentionally lightweight: it uses only the C++ standard library. There are no third-party runtime libraries, no frameworks, and no package-manager dependencies. If your compiler supports the required C++20 standard library features, the game builds from plain source.

## Features

- Main menu with game start, Hall of Fame, settings, and quit options.
- Three difficulty levels:
  - Easy: numbers from 1 to 50.
  - Medium: numbers from 1 to 100.
  - Hard: numbers from 1 to 250.
- Standard game mode with optional challenge mode.
- New Game Plus mode, where the hidden number is rerolled after a fixed number of wrong guesses:
  - Easy: every 8 wrong guesses.
  - Medium: every 7 wrong guesses.
  - Hard: every 6 wrong guesses.
- Hall of Fame with separate TOP5 rankings for each difficulty.
- Score ranking by number of attempts, then by shorter game duration.
- NG+ scores are marked in the Hall of Fame.
- Settings menu:
  - Change language between English and Polish.
  - Clear Hall of Fame after confirmation.
  - Toggle whether the game asks about challenge mode.
- Debug builds show the hidden number, which makes testing much less painful.
- The renderer uses the terminal alternate screen buffer, so the game behaves like a proper full-screen terminal app and leaves the normal shell view clean after exit.

## How to Play

Run the executable in a real terminal. The game displays numbered menu options; type the number of the option you want and press Enter.

1. Choose `Start new game`.
2. Select a difficulty.
3. Choose a game mode:
   - `Standard mode`: normal guessing game.
   - `New game plus`: the hidden number is periodically rerolled.
4. In Standard mode, the game may ask whether you want challenge mode:
   - If enabled, enter the maximum number of attempts.
   - If disabled, play without an attempt limit.
5. Enter guesses until you win or run out of attempts.
6. After a victory, enter your name to save the result in the Hall of Fame.

The current attempt number is always shown during gameplay. If the guess is too low or too high, the game answers with a random hint phrase. Invalid input does not count as a guess.

## Build Requirements

Required tools:

- A C++20 compiler with standard library support for the used C++20 features, including `std::format`.
- CMake 3.20 or newer.

Recommended compiler:

- GCC 13 or newer on Linux.

No external C++ libraries are required. The code is STL-only: all gameplay, rendering, random generation, state handling, formatting, localization, and score storage are implemented with the C++ standard library.

## Building with CMake

From the project root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Run the game:

```sh
./build/guess-the-number
```

## Debug Build

For development and testing, build in Debug mode:

```sh
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
./build-debug/guess-the-number
```

Debug builds intentionally display the hidden number on the gameplay screen:

```text
[Debug] Random number: 42
```

This happens because the debug output is compiled only when `NDEBUG` is not defined. Release builds normally define `NDEBUG`, so the secret number is hidden from the player.

## Manual Build Without CMake

If you only want to check the project quickly and already have `g++`, you can compile it directly:

```sh
g++ -std=c++20 -Iinclude src/*.cpp -o guess-the-number
./guess-the-number
```

This is useful for quick experiments, but the CMake build is the intended project build.

## Terminal Notes

The game uses ANSI escape sequences and the alternate screen buffer:

- On start, it switches to the alternate screen.
- On every redraw, it clears and redraws the current screen.
- On exit, it returns to the normal terminal screen.

This works well in normal Linux terminal emulators. If the output looks strange, run the game in a terminal with ANSI escape support instead of a minimal console or redirected output.

## Project Structure

```text
include/                 Public project headers
src/                     Source files
CMakeLists.txt           CMake build configuration
project_requirements.txt Original assignment requirements
```

The implementation is based on a small state machine. Menus, gameplay, settings, Hall of Fame, and post-game screens are represented as separate states. Transitions between states are modeled explicitly, which keeps the UI flow extendable without turning `main()` into a festival of nested `if` statements.

## Assignment Coverage

- Object-oriented C++ implementation.
- README with game instructions.
- Settings screen with language selection, Hall of Fame clearing, and challenge prompt toggle.
- Current settings shown on the settings screen.
- Hall of Fame TOP5 rankings grouped by difficulty.
- New Game Plus scores marked in Hall of Fame.
- Standard mode and New Game Plus mode selection.
- New Game Plus reroll mechanic.
- Challenge mode disabled for New Game Plus.
- Game duration shown after the game and stored in Hall of Fame entries.
- Hall of Fame sorting by attempts and then by duration.
- Difficulty selection before gameplay.
- Current attempt number shown during gameplay.
- Random hint messages for too-low and too-high guesses.
- Name input after victory and return to the main menu.
