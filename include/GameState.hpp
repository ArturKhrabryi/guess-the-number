#pragma once

#include <memory>
#include <utility>


struct GameContext;

class GameState
{
private:
    GameContext& context;

protected:
    GameContext& getContext() { return this->context; }
    const GameContext& getContext() const { return this->context; }

    template <typename State, typename... Args>
    std::unique_ptr<State> makeState(Args&&... args)
    {
        return std::make_unique<State>(this->context, std::forward<Args>(args)...);
    }

public:
    virtual ~GameState() = default;

    explicit GameState(GameContext& context) : context(context) {}
};
