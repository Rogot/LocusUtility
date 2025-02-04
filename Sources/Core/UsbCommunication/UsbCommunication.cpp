//
// UsbCommunication.cpp
//
//  Created on: Dec 20, 2024
//  Author: Maksim Sushkov
//

#include "UsbCommunication.hpp"

#include "DroneDevice/PayloadProtocol/SerialHandler.hpp"
#include "DroneDevice/StaticDeviceHub.hpp"
#include "DroneDevice/InternalDevice/InternalDevice.hpp"
#include "DroneDevice/InternalDevice/FieldList.hpp"
#include "DroneDevice/InternalDevice/VolatileField.hpp"
#include "DroneDevice/DeviceObserver.hpp"

UsbCommunication::UsbCommunication() : 
    method(ConnectionMethod::NONE),
    vid(0),
    pid(0),
    portName(""),
    devices(nullptr)
{
    initPayloadProtocol(*this);
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
        if (serial.openPort(portName) == UsbTypes::TransferErrorStatus::ERROR_OPEN) {
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
        if (serial.closePort(portName) != UsbTypes::TransferErrorStatus::SUCCESS) {
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
            std::cout << "Device USB list is empty" << std::endl;
            #endif
            return UsbResult::GET_DEVICE_LIST_ERROR;
        }
        break;
    
    case ConnectionMethod::COM:
        portList.clear();
        portList = serial.getAvailablePorts();
        if (!portList.size()) {
            #if DEBUG
            std::cout << "Device COM list is empty" << std::endl;
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

UsbTypes::TransferErrorStatus UsbCommunication::write(uint8_t *aBuffer, size_t aLength)
{
    UsbTypes::TransferErrorStatus status = UsbTypes::TransferErrorStatus::ERROR_METHOD_NOT_CHOOSE;
    switch (method)
    {
    case ConnectionMethod::USB:
        status = writeDataUsbImpl(aBuffer, aLength);
        #if DEBUG
        if (status > UsbTypes::TransferErrorStatus::SUCCESS) {
            std::cout << "Transmit Error USB" << std::endl;
        }
        #endif
        break;

    case ConnectionMethod::COM:
        status = writeDataComImpl(aBuffer, aLength);
        #if DEBUG
        if (status > UsbTypes::TransferErrorStatus::SUCCESS) {
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

UsbTypes::TransferStatus UsbCommunication::read(uint8_t *aBuffer, size_t aLength)
{
    UsbTypes::TransferStatus status = {UsbTypes::TransferErrorStatus::ERROR_METHOD_NOT_CHOOSE, 0};
    switch (method)
    {
    case ConnectionMethod::USB:
        status = readDataUsbImpl(aBuffer, aLength);
        #if DEBUG
        if (status.status > UsbTypes::TransferErrorStatus::SUCCESS) {
            std::cout << "Receive Error USB" << std::endl;
        }
        #endif
        break;

    case ConnectionMethod::COM:
        status = readDataComImpl(aBuffer, aLength);
        #if DEBUG
        if (status.status > UsbTypes::TransferErrorStatus::SUCCESS) {
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

UsbTypes::TransferErrorStatus UsbCommunication::writeDataUsbImpl(uint8_t *aBuffer, size_t aLength)
{

}

UsbTypes::TransferErrorStatus UsbCommunication::writeDataComImpl(uint8_t *aBuffer, size_t aLength)
{
    UsbTypes::TransferStatus errorStatus = {UsbTypes::TransferErrorStatus::SUCCESS, 0};
    errorStatus = serial.writeData(aBuffer, aLength);

    if (errorStatus.status > UsbTypes::TransferErrorStatus::SUCCESS || !errorStatus.bytesTransfered) {
        return UsbTypes::TransferErrorStatus::ERROR_TRANSMIT_DATA;
    }

    return UsbTypes::TransferErrorStatus::SUCCESS;
}

UsbTypes::TransferStatus UsbCommunication::readDataUsbImpl(uint8_t *aBuffer, size_t aLength)
{

}

UsbTypes::TransferStatus UsbCommunication::readDataComImpl(uint8_t *aBuffer, size_t aLength)
{
    UsbTypes::TransferStatus transferStatus = {UsbTypes::TransferErrorStatus::SUCCESS, 0};
    transferStatus = serial.readData(aBuffer, aLength);

    if (transferStatus.status > UsbTypes::TransferErrorStatus::SUCCESS || !transferStatus.bytesTransfered) {
        transferStatus.status = UsbTypes::TransferErrorStatus::ERROR_RECEIVE_DATA;
        return transferStatus;
    }

    return transferStatus;
}

bool UsbCommunication::waitForReadyRead(size_t aMs)
{
    return true;
}

void UsbCommunication::initPayloadProtocol(UsbCommunication& aUsb)
{
    // Device::DeviceId address{1};
    // Device::InternalDevice node{"LocusUtility"};

    // uint8_t rxDataSize = 0;
    // uint8_t rxBuffer[sizeof(decltype(rxDataSize))];

    // auto deviceHubHandler = Device::makeStaticDeviceHub(Device::makeDynamicHubNode(&node, address));

	// auto payloadStreamHandler = new PayloadProtocol::SerialHandler<
	// 		kMaxPacketLength, decltype(deviceHubHandler),
	// 		decltype(aUsb), FastCrc8>
	// 		(deviceHubHandler, aUsb);

    // using SerialType = decltype(aUsb);
	// using HandlerType = decltype(*payloadStreamHandler);
    
    // auto serialListener = new PayloadProtocol::SerialListener<SerialType, HandlerType>(aUsb, *payloadStreamHandler);
    
    // payloadStreamHandler->update(rxBuffer, rxDataSize);
}