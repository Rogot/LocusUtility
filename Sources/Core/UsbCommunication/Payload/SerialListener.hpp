//
// SerialListener.hpp
//
//  Created on: Dec 24, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES__CORE_USBCOMMUNICATION_SERIALLISTENER_HPP_
#define SOURCES__CORE_USBCOMMUNICATION_SERIALLISTENER_HPP_

#include <chrono>
#include <thread>
#include <mutex>

#include <Core/UsbCommunication/UsbTypes.hpp>

#define DTBUG       true

namespace PayloadProtocol {

template<typename Serial,typename Handler>
class SerialListener {
public:
	static constexpr size_t kBufferLength{256};
    static constexpr size_t maxWaitingTimeMs{4000};

public:
    SerialListener(Serial &aSerial, Handler &aHandler) : 
        serial(aSerial),
        handler(aHandler),
        mutex(),
        is_active(false),
        thread([&]() {
            run();
        })
    {
        
    }

    virtual ~SerialListener()
    {
        
    }

private:

    void start()
    {
        is_active = true;
        #if DTBUG
        std::cout << "Listener is active" << std::endl;
        #endif
        thread.join();
    }

    void stop()
    {
        is_active = false;
        #if DTBUG
        std::cout << "Listener is inactive" << std::endl;
        #endif
    }

    void run()
    {
        UsbTypes::TransferStatus transferStatus = {UsbTypes::TransferErrorStatus::SUCCESS, 0};
        #if DTBUG
        std::cout << "SerialListener run() is ON" << std::endl;
        #endif

        auto startTime = std::chrono::steady_clock::now();
        auto waitingTime = std::chrono::steady_clock::now();

        while(is_active) {
            std::lock_guard<std::mutex> lock(mutex);
            if (serial.waitForReadyRead(1)) {
				transferStatus = serial.read(serialBufferOutside, sizeof(serialBufferOutside));

                size_t delayMs = std::chrono::duration_cast<std::chrono::milliseconds>(waitingTime - startTime).count();

                if (transferStatus.status == UsbTypes::TransferErrorStatus::ERROR_METHOD_NOT_CHOOSE || 
                    delayMs >= maxWaitingTimeMs) {
                    stop();
                }

				if (transferStatus.bytesTransfered) {
                    startTime = std::chrono::steady_clock::now();
					handler.update(static_cast<void *>(serialBufferOutside), transferStatus.bytesTransfered);
				} else {
                    waitingTime = std::chrono::steady_clock::now();
                }
			}
        }
    }

private:
	uint8_t serialBufferOutside[kBufferLength];
    bool is_active;

    std::mutex mutex;
    std::thread thread;
    Handler &handler;
    Serial serial;
};

}; // PayloadProtocol

#endif // SOURCES__CORE_USBCOMMUNICATION_SERIALLISTENER_HPP_