#include "StateInterfaces.hpp"

#include <any>


bool ReturnHandler::isVoid(const std::any& value) noexcept
{
    return std::any_cast<VoidReturn>(&value) != nullptr;
}
