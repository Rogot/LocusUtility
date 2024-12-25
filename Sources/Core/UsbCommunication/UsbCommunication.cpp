#include "UsbCommunication.hpp"

#include "DroneDevice/PayloadProtocol/SerialHandler.hpp"
#include "DroneDevice/StaticDeviceHub.hpp"
#include "DroneDevice/InternalDevice/InternalDevice.hpp"
#include "DroneDevice/InternalDevice/FieldList.hpp"
#include <DroneDevice/InternalDevice/VolatileField.hpp>

UsbCommunication::UsbCommunication() : 
    method(ConnectionMethod::NONE),
    vid(0),
    pid(0),
    portName(""),
    devices(nullptr)
{
    // initPayloadProtocol(*this);
}

UsbCommunication::~UsbCommunication()
{
    if (devices) {
        delete devices;
    }
}

UsbCommunication::UsbResult UsbCommunication::connect(ConnectionMethod aMethod)
{
    switch (aMethod) {
    case ConnectionMethod::USB:
        if (!usb.connectToDevice(vid, pid)) {
            #if DEBUG
            std::cout << "idVendor: (0x" << std::setw(4) << std::hex << vid 
                << ") idProduct: (0x" << std::setw(4) << std::hex << pid 
                << ") is not available!" << std::endl;
            #endif
            return UsbResult::CONNECTION_ERROR;
        }
        #if DEBUG
        std::cout << "idVendor: (0x" << std::setw(4) << std::hex << vid 
            << ") idProduct: (0x" << std::setw(4) << std::hex << pid 
            << ") is opened!" << std::endl;
        #endif
        break;
    
    case ConnectionMethod::COM:
        if (serial.openPort(portName) == SystemSerial::ErrorStatus::ERROR_OPEN) {
            #if DEBUG
            std::cout << "Port " << portName << " is not available!" << std::endl;
            #endif
            return UsbResult::CONNECTION_ERROR;
        }
        #if DEBUG
        std::cout << "Port " << portName << " is opened!" << std::endl;
        #endif
        break;

    default:
        #if DEBUG
        std::cout << "Method didn't choose" << std::endl;
        #endif
        return UsbResult::METHOD_NOT_CHOOSE_ERROR;
        break;
    }
    
    method = aMethod;

    return UsbResult::SUCCESS;
}

UsbCommunication::UsbResult UsbCommunication::disconnect()
{
    switch (method)
    {
    case ConnectionMethod::USB:
        if (!usb.disconnectFromDevice()) {
            #if DEBUG
            std::cout << "Could not diconnect to USB" << std::endl;
            #endif
            return UsbResult::DISCONNECTION_ERROR;
        }
        break;
    
    case ConnectionMethod::COM:
        if (serial.closePort(portName) != SystemSerial::ErrorStatus::SUCCESS) {
            #if DEBUG
            std::cout << "Could not disconnect to COM port" << std::endl;
            #endif
            return UsbResult::DISCONNECTION_ERROR;
        }
        break;

    default:
        #if DEBUG
        std::cout << "Method didn't choose" << std::endl;
        #endif
        return UsbResult::METHOD_NOT_CHOOSE_ERROR;
        break;
    }

    method = ConnectionMethod::NONE;

    return UsbResult::SUCCESS;
}

UsbCommunication::UsbResult UsbCommunication::toDetermineDevices(ConnectionMethod aMethod)
{
    switch (aMethod)
    {
    case ConnectionMethod::USB:
        if (devices) {
            delete devices;
        }
        
        devices = usb.getDeviceList();
        
        if (!devices) {
            #if DEBUG
            std::cout << "Device USB list not enable" << std::endl;
            #endif
            return UsbResult::GET_DEVICE_LIST_ERROR;
        }
        break;
    
    case ConnectionMethod::COM:
        portList.clear();
        portList = serial.getAvailablePorts();
        if (!portList.size()) {
            #if DEBUG
            std::cout << "Device COM list not enable" << std::endl;
            #endif
            return UsbResult::GET_DEVICE_LIST_ERROR;
        }
        break;

    default:
        #if DEBUG
        std::cout << "Method didn't choose" << std::endl;
        #endif
        return UsbResult::METHOD_NOT_CHOOSE_ERROR;
        break;
    }
    return UsbResult::SUCCESS;
}

UsbCommunication::TransferErrorStatus UsbCommunication::write(uint8_t *aBuffer, size_t aLength)
{
    TransferErrorStatus status = TransferErrorStatus::METHOD_NOT_CHOOSE_ERROR;
    switch (method)
    {
    case ConnectionMethod::USB:
        status = writeDataUsbImpl(aBuffer, aLength);
        #if DEBUG
        if (status > TransferErrorStatus::SUCCESS) {
            std::cout << "Transmit Error USB" << std::endl;
        }
        #endif
        break;

    case ConnectionMethod::COM:
        status = writeDataComImpl(aBuffer, aLength);
        #if DEBUG
        if (status > TransferErrorStatus::SUCCESS) {
            std::cout << "Transmit Error COM" << std::endl;
        }
        #endif
        break;

    default:
        #if DEBUG
        std::cout << "Method didn't choose" << std::endl;
        #endif
        return status;
        break;
    }

    return status;
}

UsbCommunication::TransferErrorStatus UsbCommunication::read(uint8_t *aBuffer, size_t aLength)
{
    TransferErrorStatus status = TransferErrorStatus::METHOD_NOT_CHOOSE_ERROR;
    switch (method)
    {
    case ConnectionMethod::USB:
        status = readDataUsbImpl(aBuffer, aLength);
        #if DEBUG
        if (status > TransferErrorStatus::SUCCESS) {
            std::cout << "Receive Error USB" << std::endl;
        }
        #endif
        break;

    case ConnectionMethod::COM:
        status = readDataComImpl(aBuffer, aLength);
        #if DEBUG
        if (status > TransferErrorStatus::SUCCESS) {
            std::cout << "Receive Error COM" << std::endl;
        }
        #endif
        break;
    default:
        #if DEBUG
        std::cout << "Method didn't choose" << std::endl;
        #endif
        return status;
        break;
    }

    return status;
}

UsbCommunication::TransferErrorStatus UsbCommunication::writeDataUsbImpl(uint8_t *aBuffer, size_t aLength)
{

}

UsbCommunication::TransferErrorStatus UsbCommunication::writeDataComImpl(uint8_t *aBuffer, size_t aLength)
{
    SystemSerial::TransferStatus errorStatus = {SystemSerial::ErrorStatus::SUCCESS, 0};
    errorStatus = serial.writeData(aBuffer, aLength);

    if (errorStatus.status > SystemSerial::ErrorStatus::SUCCESS || !errorStatus.bytesTransferd) {
        return TransferErrorStatus::TRANSMIT_ERROR;
    }

    return TransferErrorStatus::SUCCESS;
}

UsbCommunication::TransferErrorStatus UsbCommunication::readDataUsbImpl(uint8_t *aBuffer, size_t aLength)
{

}

UsbCommunication::TransferErrorStatus UsbCommunication::readDataComImpl(uint8_t *aBuffer, size_t aLength)
{
    SystemSerial::TransferStatus errorStatus = {SystemSerial::ErrorStatus::SUCCESS, 0};
    errorStatus = serial.readData(aBuffer, aLength);

    if (errorStatus.status > SystemSerial::ErrorStatus::SUCCESS || !errorStatus.bytesTransferd) {
        return TransferErrorStatus::RECEIVE_ERROR;
    }

    return TransferErrorStatus::SUCCESS;
}

void UsbCommunication::initPayloadProtocol(UsbCommunication& aUsb)
{
    Device::DeviceId address{1};
    Device::InternalDevice node{"LocusUtility"};

    uint8_t rxDataSize = 0;
    uint8_t rxBuffer[sizeof(decltype(rxDataSize))];

    auto deviceHubHandler = Device::makeStaticDeviceHub(Device::makeDynamicHubNode(&node, address));

	auto payloadStreamHandler = new PayloadProtocol::SerialHandler<
			kMaxPacketLength, decltype(deviceHubHandler),
			decltype(aUsb), FastCrc8>
			(deviceHubHandler, aUsb);

	// using HandlerType = decltype(*payloadStreamHandler);
    
    payloadStreamHandler->update(rxBuffer, rxDataSize);
}