//
// GlobalHandlerEvents.cpp
//
//  Created on: Nov 25, 2024
//  Author: Maksim Sushkov
//

#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"

GlobalHandlerEvents::GlobalHandlerEvents()
{
    #if DEBUG
    std::cout << "Constructor GlobalHandlerEvents" << std::endl;
    #endif

}

GlobalHandlerEvents::~GlobalHandlerEvents()
{

}

void GlobalHandlerEvents::addHandler(std::string aKey, EventHandler aHandler)
{
    #if DEBUG
    std::cout << "Add handler " << aKey << std::endl;
    #endif
    handlers.emplace(aKey, aHandler);
}

GlobalHandlerEvents::HandlerEventsStatus GlobalHandlerEvents::executeHandler(std::string aKey)
{
    #if DEBUG
    std::cout << "Execute handler " << aKey << std::endl;
    #endif
    if (handlers.size()) {
        if (handlers.at(aKey)() != HandlerEventsStatus::HANDLE) {
            return HandlerEventsStatus::ERROR;
        }
    }

    return HandlerEventsStatus::HANDLE;
}