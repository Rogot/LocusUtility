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
    refBuilder(aBuilder)
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
    
    dialogConnectButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("DialogConnectButton")
    );

    dialogCloseButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("DialogCloseButton")
    );

    resetSearchPortsButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("ResetSearchPortsButton")
    );

    portsListComboTextBox = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
        aBuilder->get_object("PortsListComboTextBox")
    );

    baudratesListComboTextBox = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
        aBuilder->get_object("BaudratesListComboTextBox")
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
}

void ConnectionDialog::definitionDefaultValues()
{
    redefinitionLabeles();
    
    if (dialogCloseButton) {
        dialogCloseButton->signal_clicked().connect([&]() {
            hide();
        });
    }

    this->signal_hide().connect([&]() {
        ownerWindow->set_sensitive(true);
    }); 

    resetUsbList(portsList);
}

void ConnectionDialog::resetUsbList(std::vector<std::string> &aPortList)
{
    aPortList = systemSerial.getAvailablePorts();
    portsListComboTextBox->remove_all();
    
    for (auto port : aPortList) {
        portsListComboTextBox->append(port);
        portsListComboTextBox->set_active_text(port);
    }
}

GlobalHandlerEvents::HandlerEventsStatus ConnectionDialog::dialogHandler()
{
    // libusb_device *devices = nullptr;
    
    // if (!(devices = usb.getDeviceList())) {
    //     return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
    // }

    // // uint16_t vid = 0x4b4, pid = 0xf1;
    // uint16_t vid = 0x1234, pid = 0x0005;
    
    // if (!usb.connectToDevice(vid, pid)) {
    //     return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
    // }

    #ifdef __linux__
    std::string portName("/dev/" + portsList[0]);
    #endif
    #ifdef __MINGW32__
    std::string portName(portsList[0]);
    #endif

    if (systemSerial.openPort(portName) == SystemSerial::ErrorStatus::ERROR_OPEN) {
        return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
    }

    return GlobalHandlerEvents::HandlerEventsStatus::HANDLE;
}

void ConnectionDialog::setOwner(Gtk::Window *aOwner)
{
    ownerWindow = aOwner;
}

} // LocusBiaconWidgets