#include "UsbCommunication.hpp"

#include "DroneDevice/PayloadProtocol/SerialHandler.hpp"
#include "DroneDevice/StaticDeviceHub.hpp"

UsbCommunication::UsbCommunication() : 
    method(ConnectionMethod::NONE),
    vid(0),
    pid(0),
    portName(""),
    devices(nullptr)
{

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
            std::cout << "idVendor: (0x" << std::setw(4) << std::hex << vid 
                << ") idProduct: (0x" << std::setw(4) << std::hex << pid 
                << ") is not available!" << std::endl;
            return UsbResult::CONNECTION_ERROR;
        }

        std::cout << "idVendor: (0x" << std::setw(4) << std::hex << vid 
            << ") idProduct: (0x" << std::setw(4) << std::hex << pid 
            << ") is opened!" << std::endl;
        break;
    
    case ConnectionMethod::COM:
        if (serial.openPort(portName) == SystemSerial::ErrorStatus::ERROR_OPEN) {
            std::cout << "Port " << portName << " is not available!" << std::endl;
            return UsbResult::CONNECTION_ERROR;
        }
        std::cout << "Port " << portName << " is opened!" << std::endl;
        break;

    default:
        break;
    }
    
    if (aMethod > ConnectionMethod::NONE) {
        method = aMethod;
    }

    return UsbResult::SUCCESS;
}

UsbCommunication::UsbResult UsbCommunication::disconnect()
{
    switch (method)
    {
    case ConnectionMethod::USB:
        if (!usb.disconnectFromDevice()) {
            return UsbResult::DISCONNECTION_ERROR;
        }
        break;
    
    case ConnectionMethod::COM:
        if (serial.closePort(portName) != SystemSerial::ErrorStatus::SUCCESS) {
            return UsbResult::DISCONNECTION_ERROR;
        }
        break;

    default:
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
            return UsbResult::GET_DEVICE_LIST_ERROR;
        }
        break;
    
    case ConnectionMethod::COM:
        portList.clear();
        portList = serial.getAvailablePorts();
        if (!portList.size()) {
            return UsbResult::GET_DEVICE_LIST_ERROR;
        }
        break;

    default:
        break;
    }
    return UsbResult::SUCCESS;
}

UsbCommunication::TransferStatus UsbCommunication::write(uint8_t *aBuffer, size_t aLength)
{
    switch (method)
    {
    case ConnectionMethod::USB:
        if (writeDataUsbImpl(aBuffer, aLength) > TransferStatus::SUCCESS) {
            return TransferStatus::TRANSMIT_ERROR;
        }
        break;

    case ConnectionMethod::COM:
        if (writeDataComImpl(aBuffer, aLength) > TransferStatus::SUCCESS) {
            return TransferStatus::TRANSMIT_ERROR;
        }
    
    default:
        break;
    }

    return TransferStatus::SUCCESS;
}

UsbCommunication::TransferStatus UsbCommunication::read(uint8_t *aBuffer, size_t aLength)
{
    switch (method)
    {
    case ConnectionMethod::USB:
        if (readDataUsbImpl(aBuffer, aLength) > TransferStatus::SUCCESS) {
            return TransferStatus::TRANSMIT_ERROR;
        }
        break;

    case ConnectionMethod::COM:
        if (readDataComImpl(aBuffer, aLength) > TransferStatus::SUCCESS) {
            return TransferStatus::TRANSMIT_ERROR;
        }
    
    default:
        break;
    }

    return TransferStatus::SUCCESS;
}


UsbCommunication::TransferStatus UsbCommunication::writeDataUsbImpl(uint8_t *aBuffer, size_t aLength)
{

}

UsbCommunication::TransferStatus UsbCommunication::writeDataComImpl(uint8_t *aBuffer, size_t aLength)
{

}

UsbCommunication::TransferStatus UsbCommunication::readDataUsbImpl(uint8_t *aBuffer, size_t aLength)
{

}

UsbCommunication::TransferStatus UsbCommunication::readDataComImpl(uint8_t *aBuffer, size_t aLength)
{

}

void UsbCommunication::initPayloadProtocol()
{
    // auto deviceHubHandler = Device::makeStaticDeviceHub(Device::makeDynamicHubNode(node, address));

	// auto payloadStreamHandler = new PayloadProtocol::SerialHandler<
	// 		kMaxPacketLength, decltype(deviceHubHandler),
	// 		decltype(*communicationUart), FastCrc8>
	// 		(deviceHubHandler, *communicationUart);

	// using HandlerType = decltype(*payloadStreamHandler);
	// auto serialListener = new PayloadProtocol::SerialListener<HandlerType>(*communicationUart, *payloadStreamHandler);
}
