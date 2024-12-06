//
// ConnectionDialog.hpp
//
//  Created on: Dec 4, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CUSTOMWIDGETS_CONNECTIONDIALOG_HPP_
#define SOURCES_CUSTOMWIDGETS_CONNECTIONDIALOG_HPP_

#include <map>
#include <iostream>
#include <string>
#include <gtkmm-3.0/gtkmm/builder.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/comboboxtext.h>
#include <gtkmm-3.0/gtkmm/dialog.h>
#include <gtkmm-3.0/gtkmm/frame.h>
#include <gtkmm-3.0/gtkmm/window.h>

#include "Core/CUsb/CUsb.hpp"
#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"
#include "Core/SystemSerial/SystemSerial.hpp"

namespace LocusBiaconWidgets {

class ConnectionDialog : public Gtk::Dialog {
private:
    enum class DialogItmesText {
        CONNECTION,
        PROTOCOL,
        TYPE,
        CURRENT_TYPE_SERIAL,
        CURRENT_TYPE_TCP,
        PORT,
        BAUDRATE,
        CONNECTION_BUTTON,
        CLOSE_BUTTON
    };

    const std::map<DialogItmesText, std::string> kDialogItemTextRu = {
        {DialogItmesText::CONNECTION, "Подключение"},
        {DialogItmesText::PROTOCOL, "Протокол"},
        {DialogItmesText::TYPE, "Тип"},
        {DialogItmesText::CURRENT_TYPE_SERIAL, "Последовательный"},
        {DialogItmesText::CURRENT_TYPE_TCP, "TCP"},
        {DialogItmesText::PORT, "Порт"},
        {DialogItmesText::BAUDRATE, "Скорость в бодах"},
        {DialogItmesText::CONNECTION_BUTTON, "Подключить"},
        {DialogItmesText::CLOSE_BUTTON, "Закрыть"}
    };

    const std::map<DialogItmesText, std::string> kDialogItemTextEn = {
        {DialogItmesText::CONNECTION, "Connection"},
        {DialogItmesText::PROTOCOL, "Protocol"},
        {DialogItmesText::TYPE, "Type"},
        {DialogItmesText::CURRENT_TYPE_SERIAL, "Serial"},
        {DialogItmesText::CURRENT_TYPE_TCP, "TCP"},
        {DialogItmesText::PORT, "Port"},
        {DialogItmesText::BAUDRATE, "Baudrate"},
        {DialogItmesText::CONNECTION_BUTTON, "Connect"},
        {DialogItmesText::CLOSE_BUTTON, "Close"}
    };

    const std::map<DialogItmesText, std::string> *kCurrentDialogItemText = nullptr;

public:
    ConnectionDialog(BaseObjectType *aCobject, const Glib::RefPtr<Gtk::Builder> &aRefBuilder);

    virtual ~ConnectionDialog();

    GlobalHandlerEvents::HandlerEventsStatus dialogHundler();
    void setOwner(Gtk::Window *aOwner);

    /**
	 * @brief Update labels when change the language
	 *
	 * @return None
	 */
    void redefinitionLabeles();

protected:
    /**
	 * @brief Set standard values for an object of the MenuBar class
	 *
	 * @return None
	 */
    void definitionDefaultValues();

private:
    Glib::RefPtr<Gtk::Builder> refBuilder;

    Glib::RefPtr<Gtk::Label> connectionLabel;
    Glib::RefPtr<Gtk::Label> protocolLabel;    
    Glib::RefPtr<Gtk::Label> typeConnectionLable;
    Glib::RefPtr<Gtk::Label> currentTypeConnectionLable;
    Glib::RefPtr<Gtk::Label> portLabel;
    Glib::RefPtr<Gtk::Label> baudrateLabel;

    Glib::RefPtr<Gtk::Button> dialogConnectButton;
    Glib::RefPtr<Gtk::Button> dialogCloseButton;  
    Glib::RefPtr<Gtk::Button> resetSearchPortsButton;
    
    Glib::RefPtr<Gtk::ComboBoxText> portsListComboTextBox;
    Glib::RefPtr<Gtk::ComboBoxText> baudratesListComboTextBox;

private:
    Gtk::Window *ownerWindow;
    CUsb usb;
};

} // LocusBiaconWidgets

#endif // SOURCES_CUSTOMWIDGETS_CONNECTIONDIALOG_HPP_