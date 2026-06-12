#pragma once

#include <string>
#include <variant>


struct Event
{
    std::string value; 
};

struct EndOfInput {};

using InputResult = std::variant<Event, EndOfInput>;
