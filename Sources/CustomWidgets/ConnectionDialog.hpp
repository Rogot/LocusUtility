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
#ifdef __linux__
#include <sys/statvfs.h>
#include <termios.h>
#endif
#include <gtkmm-3.0/gtkmm/builder.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/comboboxtext.h>
#include <gtkmm-3.0/gtkmm/dialog.h>
#include <gtkmm-3.0/gtkmm/entry.h>
#include <gtkmm-3.0/gtkmm/frame.h>
#include <gtkmm-3.0/gtkmm/notebook.h>
#include <gtkmm-3.0/gtkmm/window.h>

#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"
#include "Core/UsbCommunication/UsbCommunication.hpp"

namespace LocusBiaconWidgets {

class ConnectionDialog : public Gtk::Dialog {
private:
    enum class BaudrateValue {
        BAUDRATE_2400 = 2400,
        BAUDRATE_4800 = 4800,
        BAUDRATE_9600 = 9600,
        BAUDRATE_19200 = 19200,
        BAUDRATE_38400 = 38400,
        BAUDRATE_57600 = 57600,
        BAUDRATE_115200 = 115200,
        BAUDRATE_230400 = 230400,
        BAUDRATE_460800 = 460800,
        BAUDRATE_500000 = 500000,
        BAUDRATE_576000 = 576000,
        BAUDRATE_921600 = 921600,
        BAUDRATE_1000000 = 1000000,
        BAUDRATE_2000000 = 2000000
    };

    std::vector<int> baudrateList = {
        static_cast<int>(BaudrateValue::BAUDRATE_2400),
        static_cast<int>(BaudrateValue::BAUDRATE_4800),
        static_cast<int>(BaudrateValue::BAUDRATE_9600),
        static_cast<int>(BaudrateValue::BAUDRATE_19200),
        static_cast<int>(BaudrateValue::BAUDRATE_38400),
        static_cast<int>(BaudrateValue::BAUDRATE_57600),
        static_cast<int>(BaudrateValue::BAUDRATE_115200),
        static_cast<int>(BaudrateValue::BAUDRATE_230400),
        static_cast<int>(BaudrateValue::BAUDRATE_460800),
        static_cast<int>(BaudrateValue::BAUDRATE_500000),
        static_cast<int>(BaudrateValue::BAUDRATE_576000),
        static_cast<int>(BaudrateValue::BAUDRATE_921600),
        static_cast<int>(BaudrateValue::BAUDRATE_1000000),
        static_cast<int>(BaudrateValue::BAUDRATE_2000000)
    };

    enum class DialogItmesText {
        CONNECTION,
        PROTOCOL,
        TYPE,
        CURRENT_TYPE_SERIAL,
        CURRENT_TYPE_TCP,
        PORT,
        BAUDRATE,
        CONNECTION_BUTTON,
        CLOSE_BUTTON,
        VID_LABEL,
        PID_LABEL,
        PAGE_COM_LABEL,
        PAGE_VID_PID_LABEL
    };

    enum class Pages {
        COM_PAGE = 0,
        VID_PID_PAGE = 1
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
        {DialogItmesText::CLOSE_BUTTON, "Закрыть"},
        {DialogItmesText::VID_LABEL, "ID Производителя (VID)"},
        {DialogItmesText::PID_LABEL, "ID Устройства (PID)"},
        {DialogItmesText::PAGE_COM_LABEL, "COM"},
        {DialogItmesText::PAGE_VID_PID_LABEL, "VID/PID"}
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
        {DialogItmesText::CLOSE_BUTTON, "Close"},
        {DialogItmesText::VID_LABEL, "Vendor ID"},
        {DialogItmesText::PID_LABEL, "Product ID"},
        {DialogItmesText::PAGE_COM_LABEL, "COM"},
        {DialogItmesText::PAGE_VID_PID_LABEL, "VID/PID"}
    };

    const std::map<DialogItmesText, std::string> *kCurrentDialogItemText = nullptr;

public:
    ConnectionDialog(BaseObjectType *aCobject, const Glib::RefPtr<Gtk::Builder> &aRefBuilder);

    virtual ~ConnectionDialog();
    
    void setOwner(Gtk::Window *aOwner);

    /**
	 * @brief Update labels when change the language
	 *
	 * @return None
	 */

    void redefinitionLabeles();

    /**
	 * @brief Disconnect from COM-port or USB port
	 *
	 * @return if success, than HANDLED, other - ERROR_HANDLER
	 */

    GlobalHandlerEvents::HandlerEventsStatus disconnect();

    /**
	 * @brief Connect to COM-port or USB port
	 *
	 * @return if success, than HANDLED, other - ERROR_HANDLER
	 */

    GlobalHandlerEvents::HandlerEventsStatus connect();
protected:
    /**
	 * @brief Set standard values for an object of the MenuBar class
	 *
	 * @return None
	 */
    void definitionDefaultValues();

    /**
	 * @brief Refresh enabled COM ports list 
	 *
     * @param [aPortList] array of ports
	 * @return None
	 */

    void resetUsbList(std::vector<std::string> &aPortList);

    /**
	 * @brief Set current notebook page
	 *
     * @param [aPageNum] current notebook page
	 * @return None
	 */

    void setPage(int aPageNum) { currentPage = aPageNum; }

private:
    Glib::RefPtr<Gtk::Builder> refBuilder;

    Glib::RefPtr<Gtk::Label> connectionLabel;
    Glib::RefPtr<Gtk::Label> protocolLabel;    
    Glib::RefPtr<Gtk::Label> typeConnectionLable;
    Glib::RefPtr<Gtk::Label> currentTypeConnectionLable;
    Glib::RefPtr<Gtk::Label> portLabel;
    Glib::RefPtr<Gtk::Label> baudrateLabel;
    Glib::RefPtr<Gtk::Label> vidLabel;
    Glib::RefPtr<Gtk::Label> pidLabel;
    Glib::RefPtr<Gtk::Label> connectionPortPage;
    Glib::RefPtr<Gtk::Label> connectionVidPidPage;

    Glib::RefPtr<Gtk::Entry> vidEntry;
    Glib::RefPtr<Gtk::Entry> pidEntry;

    Glib::RefPtr<Gtk::Button> dialogConnectButton;
    Glib::RefPtr<Gtk::Button> dialogCloseButton;  
    Glib::RefPtr<Gtk::Button> resetSearchPortsButton;
    
    Glib::RefPtr<Gtk::ComboBoxText> protocolTypeComboBoxText;
    Glib::RefPtr<Gtk::ComboBoxText> typeConnectionComboBoxText;
    Glib::RefPtr<Gtk::ComboBoxText> portsListComboTextBox;
    Glib::RefPtr<Gtk::ComboBoxText> baudratesListComboTextBox;

    Glib::RefPtr<Gtk::Notebook> serialNotebook;

private:
    Gtk::Window *ownerWindow;
    UsbCommunication usbCommunication;

private:
    std::vector<std::string> portsList;
    
    int currentPage;
    int idVid;
    int idPid;
    bool is_connected;
};

} // LocusBiaconWidgets

#endif // SOURCES_CUSTOMWIDGETS_CONNECTIONDIALOG_HPP_