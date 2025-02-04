//
// CUsb.hpp
//
//  Created on: Nov 26, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CORE_USBCOMMUNICATION_CUSB_CUSB_HPP_
#define SOURCES_CORE_USBCOMMUNICATION_CUSB_CUSB_HPP_

#include <cstring>
#include <iostream>
#include <iomanip>
#include <thread>

// #include <libusb-1.0/libusb.h>
#include <Core/UsbCommunication/CUsb/libusb/libusb/libusb.h>

#define CUSB_DEBUG_LIBUSB      false
#define CUSB_DEBUG             false

class CUsb {
public:
    CUsb();

    ~CUsb();

    /**
	 * @brief Connecting to the device
	 * 
     * @param [aVid] - Vendor ID of the USB device
     * @param [aPid] - Product ID of the USB device
	 * @return Connecting status: true is "OK", false is "Error"
	 */
    bool connectToDevice(int aVid, int aPid);
    
    /**
	 * @brief Disconnecting to the device
	 * 
	 * @return Disconnecting status: true is "OK", false is "Error"
	 */
    bool disconnectFromDevice();

    /**
	 * @brief Print USB information in consol
	 * 
	 * @return USB list
	 */
    libusb_device *getDeviceList();

private:
    /**
	 * @brief Print USB information in consol
	 * 
	 * @return None
	 */
    void printDeviceList(libusb_device *aDevs_list, libusb_device_descriptor aDesc);

protected:
    libusb_device **devs_list = NULL;

    libusb_device_handle *handleUsb = NULL;

    libusb_context *context = NULL;
};

#endif // SOURCES_CORE_USBCOMMUNICATION_CUSB_CUSB_HPP_