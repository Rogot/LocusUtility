//
// UsbCommunication.hpp
//
//  Created on: Dec 20, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CORE_USBCOMMUNICATION_USBCOMMUNICATION_HPP_
#define SOURCES_CORE_USBCOMMUNICATION_USBCOMMUNICATION_HPP_

#include "CUsb/CUsb.hpp"
#include "SystemSerial/SystemSerial.hpp"

// #include "DroneDevice/PayloadProtocol/SerialHandler.hpp"

class UsbCommunication {
private:
    static constexpr size_t kMaxPacketLength{256};
    static constexpr uint8_t address{1};
public:
    enum class ConnectionMethod {
        NONE,
        USB,
        COM,
    };

    enum class UsbResult {
        SUCCESS,
        CONNECTION_ERROR,
        DISCONNECTION_ERROR,
        GET_DEVICE_LIST_ERROR,
    };

    enum class TransferStatus {
        SUCCESS,
        TRANSMIT_ERROR,
        RECEIVE_ERROR,
    };

public:
    UsbCommunication();
    
    virtual ~UsbCommunication();

    /**
    * @brief Connection using choosed method
    *
    * @param [aMethod] Connection method (USB/COM)
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */

    UsbResult connect(ConnectionMethod aMethod);

    /**
    * @brief Disconnect from COM/USB
    *
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */

    UsbResult disconnect();

    UsbResult toDetermineDevices(ConnectionMethod aMethod); 

    TransferStatus write(uint8_t *aBuffer, size_t aLength);
    
    TransferStatus read(uint8_t *aBuffer, size_t aLength);

    TransferStatus writeDataUsbImpl(uint8_t *aBuffer, size_t aLength);

    TransferStatus writeDataComImpl(uint8_t *aBuffer, size_t aLength);

    TransferStatus readDataUsbImpl(uint8_t *aBuffer, size_t aLength);
    
    TransferStatus readDataComImpl(uint8_t *aBuffer, size_t aLength);

    void initPayloadProtocol();

    /* USB connection functions */
    void setVid(int aVid) { vid = aVid; }
    void setPid(int aPid) { pid = aPid; }
    libusb_device *getUsbList() { return devices; }

    /* Serial Port connection functions */
    void setPortName(std::string& aPortName) { portName = aPortName; }
    std::vector<std::string> getPortList() { return portList; }
    
private:
    ConnectionMethod method;
    CUsb usb;
    SystemSerial serial;

private:
    /* USB connection variable */
    int vid;
    int pid;
    // std::shared_ptr<libusb_device> devices; 
    libusb_device *devices;

    /* COM connection variable */
    std::string portName;
    std::vector<std::string> portList;
};

#endif // SOURCES_CORE_USBCOMMUNICATION_USBCOMMUNICATION_HPP_