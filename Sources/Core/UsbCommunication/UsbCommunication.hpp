//
// UsbCommunication.hpp
//
//  Created on: Dec 20, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CORE_USBCOMMUNICATION_USBCOMMUNICATION_HPP_
#define SOURCES_CORE_USBCOMMUNICATION_USBCOMMUNICATION_HPP_

#include "Core/UsbCommunication/CUsb/CUsb.hpp"
#include "Core/UsbCommunication/SystemSerial/SystemSerial.hpp"
#include "Core/UsbCommunication/SerialListener.hpp"

// #include "DroneDevice/PayloadProtocol/SerialHandler.hpp"

#define DEBUG       true

class UsbCommunication {
private:
    static constexpr size_t kMaxPacketLength{256};
    // static constexpr uint8_t address{1};
public:
    enum class ConnectionMethod {
        NONE,       ///> Connection method did't choose
        USB,        ///> Connection method is libusb
        COM,        ///> Connection method is COM port
    };

    enum class UsbResult {
        SUCCESS,                    ///> Success interaction
        CONNECTION_ERROR,           ///> Error during trying connect
        DISCONNECTION_ERROR,        ///> Error during trying disconnect
        GET_DEVICE_LIST_ERROR,      ///> Error during trying get device list
        METHOD_NOT_CHOOSE_ERROR,    ///> Error connection method didn't choose
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

    /**
    * @brief Find included devices
    *
    * @param [aMethod] Connection method (USB/COM)
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */

    UsbResult toDetermineDevices(ConnectionMethod aMethod); 

    /**
    * @brief Common functions for write data using USB
    *
    * @param [aBuffer] Buffer for storage data
    * @param [aLength] Buffer length
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */
    
    UsbTypes::TransferErrorStatus write(uint8_t *aBuffer, size_t aLength);
    
    /**
    * @brief Common functions for read data using USB
    *
    * @param [aBuffer] Buffer for storage data
    * @param [aLength] Buffer length
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */

    UsbTypes::TransferErrorStatus read(uint8_t *aBuffer, size_t aLength);

    bool waitForReadyRead(size_t aMs);

    static void initPayloadProtocol(UsbCommunication& aUsb);

    /* USB connection functions */
    void setVid(int aVid) { vid = aVid; }
    void setPid(int aPid) { pid = aPid; }
    libusb_device *getUsbList() { return devices; }

    /* Serial Port connection functions */
    void setPortName(std::string& aPortName) { portName = aPortName; }
    std::vector<std::string> getPortList() { return portList; }
    
protected:
    /**
    * @brief Functions for write data using USB (libusb)
    *
    * @param [aBuffer] Buffer for storage data
    * @param [aLength] Buffer length
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */

    UsbTypes::TransferErrorStatus writeDataUsbImpl(uint8_t *aBuffer, size_t aLength);

    /**
    * @brief Functions for write data using COM port
    *
    * @param [aBuffer] Buffer for storage data
    * @param [aLength] Buffer length
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */

    UsbTypes::TransferErrorStatus writeDataComImpl(uint8_t *aBuffer, size_t aLength);

    /**
    * @brief Functions for read data using USB (libusb)
    *
    * @param [aBuffer] Buffer for storage data
    * @param [aLength] Buffer length
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */

    UsbTypes::TransferErrorStatus readDataUsbImpl(uint8_t *aBuffer, size_t aLength);
    
    /**
    * @brief Functions for read data using COM port
    *
    * @param [aBuffer] Buffer for storage data
    * @param [aLength] Buffer length
    * @return if OK - UsbResult::SUCCES, other any *_ERROR
    */

    UsbTypes::TransferErrorStatus readDataComImpl(uint8_t *aBuffer, size_t aLength);

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