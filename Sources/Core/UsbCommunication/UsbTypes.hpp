//
// UsbTypes.hpp
//
//  Created on: Dec 26, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CORE_USBCOMMUNICATION_USBTYPES_HPP_
#define SOURCES_CORE_USBCOMMUNICATION_USBTYPES_HPP_

#include <stdlib.h>

namespace UsbTypes {

    enum class TransferErrorStatus {
        SUCCESS,                    ///> All good
        ERROR_OPEN,                 ///> Error open port
        ERROR_CLOSE,                ///> Error cloese port
        ERROR_TRANSMIT_DATA,        ///> Error during transmitting data
        ERROR_RECEIVE_DATA,         ///> Error during receiving data
        ERROR_METHOD_NOT_CHOOSE,    ///> Error if transfer method didn't choose
    };

    struct TransferStatus {
        TransferErrorStatus status;
        size_t bytesTransfered;

        void operator=(const TransferStatus& rhs)
        {
            this->status = rhs.status;
            this->bytesTransfered = rhs.bytesTransfered;
        }
    };

    

} // UsbTypes

#endif // SOURCES_CORE_USBCOMMUNICATION_USBTYPES_HPP_