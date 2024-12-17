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
        return SystemSerial::ErrorStatus::ERROR_OPEN;
    }
    #endif

    #ifdef __linux__
    fd = open(aPortName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        printf("Error opening port %s\n", aPortName.c_str());
        return SystemSerial::ErrorStatus::ERROR_OPEN;
    }
    #endif

    return SystemSerial::ErrorStatus::SUCCESS;
}

SystemSerial::ErrorStatus SystemSerial::closePort(std::string& aPortName)
{
    #ifdef __MINGW32__
    //Closing port
    CloseHandle(hSerial);
    #endif

    #ifdef __linux__
    int res = close(fd);
    if (res > 0) {
        return SystemSerial::ErrorStatus::ERROR_CLOSE;
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

#ifdef __linux__
std::vector<std::string> SystemSerial::getAvailablePorts()
{
    std::vector<std::string> portNames;
    std::string temp;

    std::string pathString("/dev/");
    std::filesystem::path path(pathString);
    try {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error(path.generic_string() + "doesn't exist");
        } else {
            std::cout << "\nAvailable ports:\n";
            size_t index = 1;

            for (auto de : std::filesystem::directory_iterator(path)) {
                temp = de.path();
                if (temp.find("ttyA") != std::string::npos || temp.find("ttyU") != std::string::npos) {
                    temp.erase(0, pathString.size());
                    std::cout << index++ << ") " << temp << std::endl;
                    portNames.push_back(temp);
                }
            }
        }
    } catch (const std::filesystem::filesystem_error &ex) {
        std::cout << ex.what() << std::endl;
        throw ex;
    }
    std::sort(portNames.begin(), portNames.end());
    return portNames;
}
#endif

#ifdef __MINGW32__
std::vector<std::string> SystemSerial::getAvailablePorts()
{

}
#endif