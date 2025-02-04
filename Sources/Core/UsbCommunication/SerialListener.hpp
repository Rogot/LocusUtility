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

#include <Core/UsbCommunication/UsbTypes.hpp>

#define DTBUG       true

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
    }

    void run()
    {
        UsbTypes::TransferStatus transferStatus = {UsbTypes::TransferErrorStatus::SUCCESS, 0};
        #if DTBUG
        std::cout << "SerialListener run() is ON" << std::endl;
        #endif
        while(is_active) {
            std::lock_guard<std::mutex> lock(mutex);
            if (serial.waitForReadyRead(1)) {
				transferStatus = serial.read(serialBufferOutside, sizeof(serialBufferOutside));

				if (transferStatus.bytesTransfered) {
					handler.update(static_cast<void *>(serialBufferOutside), transferStatus.bytesTransfered);
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