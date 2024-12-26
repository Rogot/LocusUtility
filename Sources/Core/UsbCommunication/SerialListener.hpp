//
// SerialListener.hpp
//
//  Created on: Dec 24, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES__CORE_USBCOMMUNICATION_SERIALLISTENER_HPP_
#define SOURCES__CORE_USBCOMMUNICATION_SERIALLISTENER_HPP_

#include <thread>
#include <mutex>

// #include <UsbCommunication/UsbCommunication.hpp>

namespace PayloadProtocol {

template<typename Serial,typename Handler>
class SerialListener {
public:
	static constexpr size_t kBufferLength{256};

public:
    SerialListener(Serial &aSerial, Handler &aHandler) : 
        serial(aSerial),
        handler(aHandler),
        mutex(),
        thread([&]() {
            run();
        })
    {
        thread.join();
    }

    virtual ~SerialListener()
    {

    }

private:
    void run()
    {
        size_t bytesRead = 0;
        std::cout << "SerialListener run() is ON" << std::endl;
        while(true) {
            std::lock_guard<std::mutex> lock(mutex);
            if (serial.waitForReadyRead(1)) {
				// bytesRead = serial.read(serialBufferOutside, sizeof(serialBufferOutside));

				if (bytesRead) {
					handler.update(static_cast<void *>(serialBufferOutside), bytesRead);
				}
			}
        }
    }

private:
	uint8_t serialBufferOutside[kBufferLength];

    std::mutex mutex;
    std::thread thread;
    Handler &handler;
    Serial serial;
};

}; // PayloadProtocol

#endif // SOURCES__CORE_USBCOMMUNICATION_SERIALLISTENER_HPP_