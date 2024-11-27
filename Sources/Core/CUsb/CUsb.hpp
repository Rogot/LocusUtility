//
// CUsb.hpp
//
//  Created on: Nov 26, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CORE_CUSB_CUSB_HPP_
#define SOURCES_CORE_CUSB_CUSB_HPP_

#include <cstring>
#include <iostream>
#include <iomanip>
#include <libusb-1.0/libusb.h>
// #include <DroneDevice/Include/DroneDevice/InternalDevice/InternalDevice.hpp>

#define CUSB_DEBUG_LIBUSB      false
#define CUSB_DEBUG             false

class CUsb {
public:
    CUsb();

    ~CUsb();

    virtual bool connectToDevice();
    
    virtual void disconnectFromDevice();

    bool getDeviceList();

private:
    void printDeviceList(libusb_device *aDevs_list, libusb_device_descriptor aDesc);

protected:
    libusb_device_handle *handleUsb;

    libusb_context *context = NULL;
};

#endif // SOURCES_CORE_CUSB_CUSB_HPP_