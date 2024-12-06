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
#include <sys/types.h>
#include <fcntl.h>
#endif

#include <string>

class SystemSerial {
private:
    enum class ErrorStatus {
        SUCCESS,                ///> All good
        ERROR_OPEN,             ///> Error open port
        ERROR_TRANSMIT_DATA,    ///> Error during transmitting data
        ERROR_RECEIVE_DATA      ///> Error during receiving data
    };

public:
    struct TransferStatus {
        ErrorStatus status;
        size_t bytesTransferd;
    };

public:
    SystemSerial() {}
    virtual ~SystemSerial();

    ErrorStatus openPort(std::string& aPortName);
    TransferStatus writeData(std::string& aDataTx, size_t aLength);
    TransferStatus readData(std::string& aDataRx, size_t aLength);

private:
    TransferStatus writeDataImpl(std::string& aDataTx, size_t aLength)
    {
        TransferStatus tranStatus = {SystemSerial::ErrorStatus::SUCCESS, 0};

        #ifdef __MINGW32__ 
        DWORD dwBytesWrite = 0; // кол-во записанных байтов

        if(!WriteFile(hSerial, aDataTx.c_str(), aLength, &dwBytesWrite, NULL)){
            printf("Write error\r\n");
            tranStatus.status = SystemSerial::ErrorStatus::ERROR_TRANSMIT_DATA;
            tranStatus.bytesTransferd = dwBytesWrite;
        }
        #endif

        #ifdef __linux__
        int iOut = write(fd, aDataTx.c_str(), 1);
        if (iOut < 0){
            printf("Write error\n");
            tranStatus.status = SystemSerial::ErrorStatus::ERROR_TRANSMIT_DATA;
            tranStatus.bytesTransferd = iOut;
        }
        #endif

        return tranStatus;
    }

    TransferStatus readDataImpl(std::string& aDataRx, size_t aLength)
    {
        #ifdef __MINGW32__ 
        if(!ReadFile(hSerial, aDataRx.c_str(), 1, &dwBytesWrite, NULL)){
        printf("Read error\r\n");
        }
        #endif
        #ifdef __linux__
        int iOut = read(fd, aDataRx.c_str(), 1);
        if (iOut < 0){
            printf("Read error\n");
        }
        #endif

        //Закрытие порта
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