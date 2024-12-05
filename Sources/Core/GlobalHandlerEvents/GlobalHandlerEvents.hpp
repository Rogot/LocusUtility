//
// GlobalHandlerEvents.hpp
//
//  Created on: Nov 25, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CORE_GLOBALHANDLEREVENTS_GLOBALHANDLEREVENTS_HPP_
#define SOURCES_CORE_GLOBALHANDLEREVENTS_GLOBALHANDLEREVENTS_HPP_

#include <functional>
#include <string>
#include <map>
#include <iostream>

#define DEBUG       false

class GlobalHandlerEvents {
public:
    enum class HandlerEventsStatus {
        HANDLE,
        ERROR
    };

public:
    GlobalHandlerEvents();

    virtual ~GlobalHandlerEvents();

    using EventHandler = std::function<GlobalHandlerEvents::HandlerEventsStatus(void)>;

    /**
	 * @brief Add handler for some event
	 *
     * @param [aKey] The key by which the desired function will be located
     * @param [aHandler] A function executed by a key
	 * @return None
	 */
    void addHandler(std::string aKey, EventHandler aHandler);

    /**
	 * @brief Execute the function by key
	 *
     * @param [aKey] The key by which the desired function will be located
	 * @return Handler events status is HANDLE if all good, other is ERROR
	 */
    HandlerEventsStatus executeHandler(std::string aKey);

private:
    std::map<std::string, EventHandler> handlers;
};

#endif // SOURCES_CORE_GLOBALHANDLEREVENTS_GLOBALHANDLEREVENTS_HPP_