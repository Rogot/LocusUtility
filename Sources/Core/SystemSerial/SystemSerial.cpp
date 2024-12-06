//
// SystemSerial.cpp
//
//  Created on: Dec 6, 2024
//  Author: Maksim Sushkov
//

#include <Core/SystemSerial/SystemSerial.hpp>

SystemSerial::ErrorStatus SystemSerial::openPort(std::string& aPortName)
{
    #ifdef __MINGW32__
    hSerial = CreateFile(aPortName.c_str(), GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening port %s\n", aPortName.c_str());
        return SystemSerial::ErrorStatus::ERROR_OPEN
    }
    #endif

    #ifdef __linux__
    fd = open(aPortName.c_str(), O_RDWR | O_NOCTTY );
    if (fd < 0) {
        printf("Error opening port %s\n", aPortName.c_str());
        return SystemSerial::ErrorStatus::ERROR_OPEN;
    }
    #endif

    return SystemSerial::ErrorStatus::SUCCESS;
}

SystemSerial::TransferStatus SystemSerial::writeData(std::string& aDataTx, size_t aLength)
{
    return writeDataImpl(aDataTx, aLength);
}

SystemSerial::TransferStatus SystemSerial::readData(std::string& aDataRx, size_t aLength)
{
    return readDataImpl(aDataRx, aLength);
}