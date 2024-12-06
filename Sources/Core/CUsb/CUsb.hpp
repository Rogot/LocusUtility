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
#include <Core/CUsb/libusb/libusb/libusb.h>
#include <thread>


#define CUSB_DEBUG_LIBUSB      false
#define CUSB_DEBUG             false

class CUsb {
public:
    CUsb();

    ~CUsb();

    virtual bool connectToDevice(uint16_t aVid, uint16_t aPid);
    
    virtual void disconnectFromDevice();

    libusb_device *getDeviceList();

private:
    void printDeviceList(libusb_device *aDevs_list, libusb_device_descriptor aDesc);

protected:
    libusb_device **devs_list = NULL;

    libusb_device_handle *handleUsb = NULL;

    libusb_context *context = NULL;
};

#endif // SOURCES_CORE_CUSB_CUSB_HPP_