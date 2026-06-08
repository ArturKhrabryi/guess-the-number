#pragma once

#include <concepts>
#include <random>
#include <utility>


class RandomGenerator
{
private:
    std::mt19937 gen{ std::random_device{}() };

public:
    template <typename T>
    requires (std::integral<T> || std::floating_point<T>)
    [[nodiscard]] T generate(T min, T max)
    {
        if (min > max)
            std::swap(min, max);

        if constexpr (std::integral<T>)
        {
            std::uniform_int_distribution<T> dist(min, max);

            return dist(this->gen);
        }  
        else
        {
            std::uniform_real_distribution<T> dist(min, max);

            return dist(this->gen);
        }
    }
};
