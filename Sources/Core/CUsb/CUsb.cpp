//
// CUsb.cpp
//
//  Created on: Nov 26, 2024
//  Author: Maksim Sushkov
//

#include "Core/CUsb/CUsb.hpp"

CUsb::CUsb() :
    devs_list(nullptr)
{
    int res = 0;
    res = libusb_init(&context);
#ifdef CY_VID_PID
    res =libusb_set_option(context, LIBUSB_OPTION_USE_USBDK);
#endif
    if (res < 0) {
        #if CUSB_DEBUG
        std::cout << "Failed to initialise libusb\n";
        #endif
    } else {
        #if CUSB_DEBUG
        std::cout << "<libusb> was initialised\n";
        #endif
        #if CUSB_DEBUG_LIBUSB
        libusb_set_option(context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
        #endif
        handleUsb = 0;
    }
}

CUsb::~CUsb()
{
    libusb_free_device_list(devs_list, 1); //free the list, unref the devices in it
    disconnectFromDevice();
    libusb_exit(context);
}

bool CUsb::connectToDevice(uint16_t aVid, uint16_t aPid)
{
// #ifdef CY_VID_PID
//     handleUsb = libusb_open_device_with_vid_pid(context, aVid, aPid);
// #else
    handleUsb = libusb_open_device_with_vid_pid(context, aVid, aPid);
// #endif
    if (handleUsb == 0) {
        #if CUSB_DEBUG
        std::cout << "Cannot Open USB Device" << std::endl;
        #endif
        return false;
    } else {
        #if CUSB_DEBUG
        std::cout << "USB Device Opened" << std::endl;
        #endif
    }
    int res = libusb_claim_interface(handleUsb, 0);
    if (res != 0) {
        #if CUSB_DEBUG
        std::cout << "Cannot claim interface - " << libusb_error_name(res) << std::endl;
        #endif
    } else {
        #if CUSB_DEBUG
        std::cout << "Claim interface success - " << libusb_error_name(res) << std::endl;
        #endif
    }
    return true;
}

bool CUsb::disconnectFromDevice()
{
    if (handleUsb != 0)
    {
        libusb_release_interface(handleUsb, 0);
        libusb_close(handleUsb);
        handleUsb = 0;
        return true;
    }
    return false;
}

libusb_device *CUsb::getDeviceList()
{
    size_t cnt = libusb_get_device_list(context, &devs_list);

    if (cnt <= 0) {
        return nullptr;
    }

    //print total number of usb devices
    ssize_t i; //for iterating through the list
    std::cout << "\n\nUSB Device List:\n";

    for(i = 0; i < cnt; ++i) {
        struct libusb_device_descriptor desc;
        if (libusb_get_device_descriptor(devs_list[i], &desc) >= 0) {
            printDeviceList(devs_list[i], desc);

            // if ((desc.idVendor == 0x04D8) && (desc.idProduct == 0x0204)){
            //     libusb_free_device_list(devs_list, 1);
            //     return true;
            // }
        }
    }
    // if (handleUsb) libusb_close(handleUsb);
    return *devs_list;
}

void CUsb::printDeviceList(libusb_device *aDevs_list, libusb_device_descriptor aDesc)
{
    unsigned char str[2048] = {0};

    const char *speed = nullptr;

    // switch (libusb_get_device_speed(aDevs_list)) {
	// case LIBUSB_SPEED_LOW:		speed = "1.5M"; break;
	// case LIBUSB_SPEED_FULL:		speed = "12M"; break;
	// case LIBUSB_SPEED_HIGH:		speed = "480M"; break;
	// case LIBUSB_SPEED_SUPER:	speed = "5G"; break;
	// case LIBUSB_SPEED_SUPER_PLUS:	speed = "10G"; break;
	// case LIBUSB_SPEED_SUPER_PLUS_X2:	speed = "20G"; break;
	// default:			speed = "Unknown";
	// }

    std::cout << "-- "
        << "Bus " << std::setfill('0') << std::setw(3) << std::hex << +libusb_get_bus_number(aDevs_list) << " "
        << "Device " << std::setfill('0') << std::setw(3) << std::hex << +libusb_get_device_address(aDevs_list) << ": "
        << "ID " << std::setfill('0') << std::setw(4) << std::hex << aDesc.idVendor << ":"
        << std::setfill('0') << std::setw(4) << std::hex << aDesc.idProduct << "::";
        if (speed != nullptr) {
            std::cout << " Speed " << std::setfill('0') << std::setw(4) << std::hex << speed;
        }

    if (libusb_open(aDevs_list, &handleUsb) == LIBUSB_SUCCESS) {
        if (aDesc.idVendor) {
            if (libusb_get_string_descriptor_ascii(handleUsb, aDesc.idVendor, str, 2048) > 0) {
                std::string s(reinterpret_cast<char*>(str));
                // std::cout << " " << s.substr(s.find_first_not_of(" "));
                std::string test("TEST P:");
                std::cout << " " << test << " " << s;
            }
        }
        if (aDesc.idProduct) {
            if (libusb_get_string_descriptor_ascii(handleUsb, aDesc.idProduct, str, 2048) > 0) {
                std::string s(reinterpret_cast<char*>(str));
                // std::cout << " " << s.substr(s.find_first_not_of(" "));
                std::string test("TEST V:");
                std::cout << " " << test << " " << s;
            }
        }
        if (aDesc.iProduct) {
            if (libusb_get_string_descriptor_ascii(handleUsb, aDesc.iProduct, str, 2048) > 0) {
                std::string s(reinterpret_cast<char*>(str));
                std::cout << " " << s.substr(s.find_first_not_of(" "));
            }
        }
        if (aDesc.iManufacturer) {
			if (libusb_get_string_descriptor_ascii(handleUsb, aDesc.iManufacturer, str, 2048) > 0) {
                std::string s(reinterpret_cast<char*>(str));
                std::cout << " " << s.substr(s.find_first_not_of(" "));
            }
		}
    } else { 
        std::cout << " -";
    }
    std::cout << std::endl;
}