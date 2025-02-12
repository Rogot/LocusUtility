//
// ConnectionDialog.cpp
//
//  Created on: Dec 4, 2024
//  Author: Maksim Sushkov
//

#include "CustomWidgets/ConnectionDialog.hpp"

namespace LocusBiaconWidgets {

ConnectionDialog::ConnectionDialog(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder) : 
    Glib::ObjectBase("ConnectionDialogLB"),
    Gtk::Dialog(aCobject),
    refBuilder(aBuilder),
    currentPage(0),
    idPid(0),
    idVid(0),
    usbCommunication{nullptr},
    is_connected(false)
{
    set_title("Connection");
    
    typeConnectionLable = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("TypeConnectionLable")
    );

    currentTypeConnectionLable = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("CurrentTypeConnectionLable")
    );

    portLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("PortLabel")
    );

    baudrateLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("BaudrateLabel")
    );

    connectionLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("ConnectionLabel")
    );

    protocolLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("ProtocolLabel")
    );

    vidLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("VidLabel")
    );

    pidLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("PidLabel")
    );

    connectionPortPage = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("ConnectionPortPage")
    );

    connectionVidPidPage = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("ConnectionVidPidPage")
    );
    
    dialogConnectButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("DialogConnectButton")
    );

    dialogCloseButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("DialogCloseButton")
    );

    resetSearchPortsButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("ResetSearchPortsButton")
    );

    typeConnectionComboBoxText = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
        aBuilder->get_object("TypeConnectionComboBoxText")
    );

    portsListComboTextBox = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
        aBuilder->get_object("PortsListComboTextBox")
    );

    baudratesListComboTextBox = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
        aBuilder->get_object("BaudratesListComboTextBox")
    );

    protocolTypeComboBoxText = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
        aBuilder->get_object("ProtocolTypeComboBoxText")
    );

    vidEntry = Glib::RefPtr<Gtk::Entry>::cast_dynamic(
        aBuilder->get_object("VidEntry")
    );

    pidEntry = Glib::RefPtr<Gtk::Entry>::cast_dynamic(
        aBuilder->get_object("PidEntry")
    );
    
    serialNotebook = Glib::RefPtr<Gtk::Notebook>::cast_dynamic(
        aBuilder->get_object("SerialNotebook")
    );

    definitionDefaultValues();
}

ConnectionDialog::~ConnectionDialog()
{
    ownerWindow->set_sensitive(true);
}

void ConnectionDialog::redefinitionLabeles()
{
    // Change Language
    if (kCurrentDialogItemText != nullptr) {
        if (kCurrentDialogItemText == &kDialogItemTextRu) {
            kCurrentDialogItemText = &kDialogItemTextEn;
        } else if (kCurrentDialogItemText == &kDialogItemTextEn) {
            kCurrentDialogItemText = &kDialogItemTextRu;
        }
    } else {
        kCurrentDialogItemText = &kDialogItemTextRu;
    }

    // Redefinition
    if (typeConnectionLable) {
        typeConnectionLable->set_label(kCurrentDialogItemText->at(DialogItmesText::TYPE).c_str());
    }

    //TO DO: сделать выбор варианта в зависимости от способа подключения
    if (currentTypeConnectionLable) {
        currentTypeConnectionLable->set_label(kCurrentDialogItemText->at(DialogItmesText::CURRENT_TYPE_SERIAL).c_str());
    }

    if (portLabel) {
        portLabel->set_label(kCurrentDialogItemText->at(DialogItmesText::PORT).c_str());
    }

    if (baudrateLabel) {
        baudrateLabel->set_label(kCurrentDialogItemText->at(DialogItmesText::BAUDRATE).c_str());
    }

    if (dialogConnectButton) {
        dialogConnectButton->set_label(kCurrentDialogItemText->at(DialogItmesText::CONNECTION_BUTTON).c_str());
    }

    if (dialogCloseButton) {
        dialogCloseButton->set_label(kCurrentDialogItemText->at(DialogItmesText::CLOSE_BUTTON).c_str());
    }

    if (connectionLabel) {
        connectionLabel->set_label(kCurrentDialogItemText->at(DialogItmesText::CONNECTION).c_str());
    }
    
    if (protocolLabel) {
        protocolLabel->set_label(kCurrentDialogItemText->at(DialogItmesText::PROTOCOL).c_str());
    }

    if (vidLabel) {
        vidLabel->set_label(kCurrentDialogItemText->at(DialogItmesText::VID_LABEL).c_str());
    }

    if (pidLabel) {
        pidLabel->set_label(kCurrentDialogItemText->at(DialogItmesText::PID_LABEL).c_str());
    }

    if (connectionPortPage) {
        connectionPortPage->set_label(kCurrentDialogItemText->at(DialogItmesText::PAGE_COM_LABEL).c_str());
    }

    if (connectionVidPidPage) {
        connectionVidPidPage->set_label(kCurrentDialogItemText->at(DialogItmesText::PAGE_VID_PID_LABEL).c_str());
    }
}

void ConnectionDialog::definitionDefaultValues()
{
    redefinitionLabeles();
    resetUsbList(portsList);

    if (typeConnectionComboBoxText) {
        std::string text = kDialogItemTextEn.at(DialogItmesText::CURRENT_TYPE_TCP).c_str();
        typeConnectionComboBoxText->append(text);
        text = kDialogItemTextEn.at(DialogItmesText::CURRENT_TYPE_SERIAL).c_str();
        typeConnectionComboBoxText->append(text);
        typeConnectionComboBoxText->set_active_text(text);
    }

    if (protocolTypeComboBoxText) {
        std::string text = "Payload";
        protocolTypeComboBoxText->append(text);
        protocolTypeComboBoxText->set_active_text(text);
    }

    if (baudratesListComboTextBox) {
        baudratesListComboTextBox->remove_all();
        for (auto baudrate : baudrateList) {
            baudratesListComboTextBox->append(std::to_string(baudrate));
        }
        baudratesListComboTextBox->set_active_text(std::to_string(static_cast<int>(BaudrateValue::BAUDRATE_115200)));
    }

    if (dialogCloseButton) {
        /**
        * @brief Close dialog window
        *
        * @return None 
        */
        dialogCloseButton->signal_clicked().connect([&]() {
            hide();
        });
    }

        
    if (dialogConnectButton) {
        /**
        * @brief Connect to USB device
        *
        * @return None 
        */
        dialogConnectButton->signal_clicked().connect([&]() {
            this->connect();
            hide();
        });
    }

    if (resetSearchPortsButton) {
        /**
        * @brief Reset USB list
        *
        * @return None 
        */
        resetSearchPortsButton->signal_clicked().connect([&]() {
            this->resetUsbList(portsList);
        });
    }

    if (serialNotebook) {
        /**
         * @brief Defines the USB connection method depending on the current window and connection status
         *
         * @param [aPage] Pointer of page widget
         * @param [aPageNumber] Page representation as a number
         * @return None
         * 
         * @note If "is_connected" == false, then you can switch a method
         */
        serialNotebook->signal_switch_page().connect([&](Gtk::Widget* aPage, guint aPageNumber) {
            if (!is_connected) {
                this->setPage(static_cast<int>(aPageNumber));
            }
        });
    }

    if (vidEntry) {
        /**
         * @brief Read data from "VID" field depending on the connection status
         *
         * @return None
         * 
         * @note If "is_connected" == false, then you can read data
         */
        vidEntry->signal_activate().connect([&]() {
            if (!is_connected) {
                std::string entryValue = vidEntry->get_chars(0, 5);
                idVid = std::stoi(entryValue, 0, 16);
            }
        });
    }

    if (pidEntry) {
        /**
         * @brief Read data from "PID" field depending on the connection status
         *
         * @return None
         * 
         * @note If "is_connected" == false, then you can read data
         */
        pidEntry->signal_activate().connect([&]() {
            if (!is_connected) {
                std::string entryValue = pidEntry->get_chars(0, 5);
                idPid = std::stoi(entryValue, 0, 16);
            }
        });
    }

    /**
    * @brief Makes the parent window available for interaction when the dialog is closed
    *
    * @return None 
    */
    this->signal_hide().connect([&]() {
        ownerWindow->set_sensitive(true);
    });
}

GlobalHandlerEvents::HandlerEventsStatus ConnectionDialog::connect()
{
    if (!usbCommunication) {
        return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
    }

    #ifdef __linux__
    std::string portName;
    struct stat sb;
    if (!is_connected) {
        if (portsList.size() > 0) {
            portName = "/dev/" + portsListComboTextBox->get_active_text();
            stat(portName.c_str(), &sb);
        }

        if (currentPage == static_cast<int>(Pages::VID_PID_PAGE)) {
            usbCommunication->setVid(idVid);
            usbCommunication->setPid(idPid);

            if (usbCommunication->connect(UsbCommunication::ConnectionMethod::USB) != UsbCommunication::UsbResult::SUCCESS) {
                return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
            }
        } else if (currentPage == static_cast<int>(Pages::COM_PAGE)) {
            usbCommunication->setPortName(portName);

            if (usbCommunication->connect(UsbCommunication::ConnectionMethod::COM) != UsbCommunication::UsbResult::SUCCESS) {
                return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
            }
        }
        #endif
        #ifdef __MINGW32__
        std::string portName(portsList[0]);
        #endif

        is_connected = true;
    }
    return GlobalHandlerEvents::HandlerEventsStatus::HANDLED;
}

GlobalHandlerEvents::HandlerEventsStatus ConnectionDialog::disconnect()
{   
    if (!usbCommunication) {
        return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
    }
    
    if (is_connected) {
        if (usbCommunication->disconnect() != UsbCommunication::UsbResult::SUCCESS) {
            return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
        }

        is_connected = false;
    }
    return GlobalHandlerEvents::HandlerEventsStatus::HANDLED;
} 

void ConnectionDialog::resetUsbList(std::vector<std::string> &aPortList)
{
    if (!usbCommunication) {
        return;
    }

    usbCommunication->toDetermineDevices(UsbCommunication::ConnectionMethod::COM);
    aPortList = usbCommunication->getPortList();
    if (portsListComboTextBox) {
        portsListComboTextBox->remove_all(); 
        if (aPortList.size()) {
            for (auto port : aPortList) {
                portsListComboTextBox->append(port);
                portsListComboTextBox->set_active_text(port);
            }
        } else {
            std::string port("None");
            portsListComboTextBox->append(port);
            portsListComboTextBox->set_active_text(port);
        }
    }
}

void ConnectionDialog::setOwner(Gtk::Window *aOwner)
{
    ownerWindow = aOwner;
}

void ConnectionDialog::setUsbConnetctionPtr(UsbCommunication &aUsbPtr) 
{
    usbCommunication = &aUsbPtr;
    resetUsbList(portsList); 
}

} // LocusBiaconWidgets