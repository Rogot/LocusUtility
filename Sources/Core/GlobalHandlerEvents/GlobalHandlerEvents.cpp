//
// GlobalHandlerEvents.cpp
//
//  Created on: Nov 25, 2024
//  Author: Maksim Sushkov
//

#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"

GlobalHandlerEvents::GlobalHandlerEvents()
{

}

GlobalHandlerEvents::~GlobalHandlerEvents()
{

}

void GlobalHandlerEvents::addHandler(std::string aKey, EventHandler aHandler)
{
    handlers.emplace(aKey, aHandler);
}

GlobalHandlerEvents::HandlerEventsStatus GlobalHandlerEvents::executeHandler(std::string aKey)
{
    if (handlers.size()) {
        if (handlers.at(aKey)() != HandlerEventsStatus::HANDLE) {
            return HandlerEventsStatus::ERROR;
        }
    }

    return HandlerEventsStatus::HANDLE;
}