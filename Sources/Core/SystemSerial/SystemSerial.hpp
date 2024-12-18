//
// SystemSerial.hpp
//
//  Created on: Dec 6, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CORE_SYSTEMSERIAL_SYSTEMSERIAL_HPP
#define SOURCES_CORE_SYSTEMSERIAL_SYSTEMSERIAL_HPP

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

class SystemSerial {
public:
    enum class ErrorStatus {
        SUCCESS,                ///> All good
        ERROR_OPEN,             ///> Error open port
        ERROR_CLOSE,            ///> Error cloese port
        ERROR_TRANSMIT_DATA,    ///> Error during transmitting data
        ERROR_RECEIVE_DATA      ///> Error during receiving data
    };

    struct TransferStatus {
        ErrorStatus status;
        size_t bytesTransferd;
    };

public:
    SystemSerial() {}
    virtual ~SystemSerial() {}

    /**
	 * @brief Get list of available USB ports (ttyACM* or ttyUSB*)
	 * 
	 * @return List of ports
	 */
    std::vector<std::string> getAvailablePorts();

    /**
	 * @brief Try open serial port (ttyACM* or ttyUSB*)
	 * 
     * @param [aPortName] - port name
	 * @return ErrorStatus is SUCCESS if ok, other - there is a error
     * 
     * @note Using port name + path to the file (in Linux)
	 */
    ErrorStatus openPort(std::string& aPortName);
    
    /**
	 * @brief Try close serial port (ttyACM* or ttyUSB*)
	 * 
     * @param [aPortName] - port name
	 * @return ErrorStatus is SUCCESS if ok, other - there is a error
	 */
    ErrorStatus closePort(std::string& aPortName);
    
    /**
	 * @brief Write data using serial port
	 * 
     * @param [aDataTx] - data for transmitting
     * @param [aLength] - data length for transmitting
	 * @return ErrorStatus is SUCCESS if ok, other - there is a error
	 */
    TransferStatus writeData(std::string& aDataTx, size_t aLength);
    
    /**
	 * @brief Read data from serial port
	 * 
     * @param [aDataRx] - received data
     * @param [aLength] - data length
	 * @return ErrorStatus is SUCCESS if ok, other - there is a error
	 */
    TransferStatus readData(std::string& aDataRx, size_t aLength);

private:
    TransferStatus writeDataImpl(std::string& aDataTx, size_t aLength)
    {
        TransferStatus tranStatus = {SystemSerial::ErrorStatus::SUCCESS, 0};

        #ifdef __MINGW32__ 
        DWORD dwBytesWrite = 0; // the number of the writnig bytes

        if(!WriteFile(hSerial, aDataTx.c_str(), aLength, &dwBytesWrite, NULL)) {
            printf("Write error\r\n");
            tranStatus.status = SystemSerial::ErrorStatus::ERROR_TRANSMIT_DATA;
        }
        tranStatus.bytesTransferd = dwBytesWrite;
        #endif

        #ifdef __linux__
        int iOut = write(fd, &aDataTx, 1);
        if (iOut < 0) {
            printf("Write error\n");
            tranStatus.status = SystemSerial::ErrorStatus::ERROR_TRANSMIT_DATA;
        }
        tranStatus.bytesTransferd = iOut;
        #endif

        return tranStatus;
    }

    TransferStatus readDataImpl(std::string& aDataRx, size_t aLength)
    {
        TransferStatus tranStatus = {SystemSerial::ErrorStatus::SUCCESS, 0};

        #ifdef __MINGW32__
        DWORD dwBytesWrite = 0; // the number of the writnig bytes
        // if(!ReadFile(hSerial, aDataRx.c_str(), 1, &dwBytesWrite, NULL)) {
        //     printf("Read error\r\n");
        //     CloseHandle(hSerial);
        //     tranStatus.status = SystemSerial::ErrorStatus::ERROR_RECEIVE_DATA;
        // }
        tranStatus.bytesTransferd = dwBytesWrite;
        #endif
        #ifdef __linux__
        int iOut = read(fd, &aDataRx, 1);
        if (iOut < 0) {
            printf("Read error\n");
            tranStatus.status = SystemSerial::ErrorStatus::ERROR_RECEIVE_DATA;
        }
        tranStatus.bytesTransferd = iOut;
        #endif

        //Closing port
        #ifdef __MINGW32__ 
        CloseHandle(hSerial);
        #endif
        #ifdef __linux
        close(fd);
        #endif
    }

private:
    #ifdef __MINGW32__
    HANDLE hSerial;
    #endif
    #ifdef __linux__
    int fd;
    #endif
};

#endif // SOURCES_CORE_SYSTEMSERIAL_SYSTEMSERIAL_HPP