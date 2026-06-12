#include "InputHandler.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <utility>


void InputHandler::trim(std::string& str)
{
    auto isNotSpace = [](unsigned char ch) { return !std::isspace(ch); };
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), isNotSpace));
    str.erase(std::find_if(str.rbegin(), str.rend(), isNotSpace).base(), str.end());
}

InputResult InputHandler::readEvent()
{ 
    std::string line;
    if (!std::getline(std::cin, line))
        return EndOfInput{};

    trim(line);

    return Event{ .value = std::move(line) };
}
