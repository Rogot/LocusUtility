//
// SystemSerial.hpp
//
//  Created on: Dec 6, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CORE_USBCOMMUNICATION_SYSTEMSERIAL_SYSTEMSERIAL_HPP_
#define SOURCES_CORE_USBCOMMUNICATION_SYSTEMSERIAL_SYSTEMSERIAL_HPP_

#ifdef __MINGW32__
#include <windows.h>
#endif
#ifdef __linux__
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "Core/UsbCommunication/UsbTypes.hpp"

class SystemSerial {
public:
    SystemSerial() : fd(-1) {}
    virtual ~SystemSerial() {}

    /**
	 * @brief Get list of available USB ports (ttyACM* or ttyUSB* for Linux)
	 * 
	 * @return List of ports
	 */
    std::vector<std::string> getAvailablePorts();

    /**
	 * @brief Try open serial port (ttyACM* or ttyUSB* for Linux)
	 * 
     * @param [aPortName] - port name
	 * @return ErrorStatus is SUCCESS if ok, other - there is a error
     * 
     * @note Using port name + path to the file (in Linux)
	 */
    UsbTypes::TransferErrorStatus openPort(std::string& aPortName);
    
    /**
	 * @brief Try close serial port (ttyACM* or ttyUSB* for Linux)
	 * 
     * @param [aPortName] - port name
	 * @return ErrorStatus is SUCCESS if ok, other - there is a error
	 */
    UsbTypes::TransferErrorStatus closePort(std::string& aPortName);
    
    /**
	 * @brief Write data using serial port
	 * 
     * @param [aDataTx] - data for transmitting
     * @param [aLength] - data length for transmitting
	 * @return ErrorStatus is SUCCESS if ok, other - there is a error
	 */
    UsbTypes::TransferStatus writeData(uint8_t *aDataTx, size_t aLength);
    
    /**
	 * @brief Read data from serial port
	 * 
     * @param [aDataRx] - received data
     * @param [aLength] - data length
	 * @return ErrorStatus is SUCCESS if ok, other - there is a error
	 */
    UsbTypes::TransferStatus readData(uint8_t *aDataRx, size_t aLength);

private:
    UsbTypes::TransferStatus writeDataImpl(uint8_t *aDataTx, size_t aLength)
    {
        UsbTypes::TransferStatus tranStatus = {UsbTypes::TransferErrorStatus::SUCCESS, 0};

        #ifdef __MINGW32__ 
        DWORD dwBytesWrite = 0; // the number of the writnig bytes

        if(!WriteFile(hSerial, aDataTx.c_str(), aLength, &dwBytesWrite, NULL)) {
            printf("Write error\r\n");
            tranStatus.status = SystemSerial::ErrorStatus::ERROR_TRANSMIT_DATA;
        }
        tranStatus.bytesTransfered = dwBytesWrite;
        #endif

        #ifdef __linux__
        long int iOut = write(fd, &aDataTx, 1);
        if (iOut < 0) {
            printf("Write error\n");
            tranStatus.status = UsbTypes::TransferErrorStatus::ERROR_TRANSMIT_DATA;
        }
        tranStatus.bytesTransfered = iOut;
        #endif

        return tranStatus;
    }

    UsbTypes::TransferStatus readDataImpl(uint8_t *aDataRx, size_t aLength)
    {
        UsbTypes::TransferStatus tranStatus = {UsbTypes::TransferErrorStatus::SUCCESS, 0};

        #ifdef __MINGW32__
        DWORD dwBytesWrite = 0; // the number of the writnig bytes
        if(!ReadFile(hSerial, aDataRx.c_str(), 1, &dwBytesWrite, NULL)) {
            printf("Read error\r\n");
            CloseHandle(hSerial);
            tranStatus.status = SystemSerial::ErrorStatus::ERROR_RECEIVE_DATA;
        }
        tranStatus.bytesTransfered = dwBytesWrite;
        #endif
        #ifdef __linux__
        long int iOut = read(fd, &aDataRx, 1);
        if (iOut < 0) {
            printf("Read error\n");
            tranStatus.status = UsbTypes::TransferErrorStatus::ERROR_RECEIVE_DATA;
        }
        tranStatus.bytesTransfered = iOut;
        #endif

        // //Closing port
        // #ifdef __MINGW32__ 
        // CloseHandle(hSerial);
        // #endif
        // #ifdef __linux
        // close(fd);
        // #endif


        return tranStatus;
    }

private:
    #ifdef __MINGW32__
    HANDLE hSerial;
    #endif
    #ifdef __linux__
    int fd;
    #endif
};

#endif // SOURCES_CORE_USBCOMMUNICATION_SYSTEMSERIAL_SYSTEMSERIAL_HPP_