//
// MainWindow.hpp
//
//  Created on: Nov 19, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_MAINWINDOW_HPP_
#define SOURCES_MAINWINDOW_HPP_

#include <iostream>
#include <string>
#include <gtkmm-3.0/gtkmm/applicationwindow.h>
#include <gtkmm-3.0/gtkmm/builder.h>

#include "CustomWidgets/ConnectionDialog.hpp"
#include "CustomWidgets/FirmwareMethodBox.hpp"
#include "CustomWidgets/MenuBar.hpp"
#include "CustomWidgets/OpenFileDialog.hpp"
#include "CustomWidgets/SelectFirmwareBox.hpp"
#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"

#define DEBUG       false

static const std::string kUiPath = {"../Sources/UserInterface.glade"};

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow(BaseObjectType *aCobject, const Glib::RefPtr<Gtk::Builder> &aRefBuilder);

    virtual ~MainWindow();

	/**
	 * @brief Creating an instance of the MainWindow class 
     * with subsequent initialization of widgets related to this window
	 *
	 * @param [aTitle] Title for the window
     * @param [aApp] Instance of the class Gtk::Application
	 * @return Pointer an instance of the MainWindow class
	 */

    static MainWindow *getInstance(const char* aTitle, Glib::RefPtr<Gtk::Application> &aApp);

    /**
	 * @brief Initialisation of the window widgets
	 *
	 * @param [aBuilder] Builder for the interface from an XML UI definition description
     * @param [aApp] Instance of the class Gtk::Application
	 * @return None
	 */

    void initWidgets(Glib::RefPtr<Gtk::Builder> &aBuilder, Glib::RefPtr<Gtk::Application> &aApp);

private:

    /**
	 * @brief Function for changing language in all app
	 *
	 * @return HandlerEventsStatus: any error is "ERROR_HANDLER", succes is "HANDLED"
	 */

    GlobalHandlerEvents::HandlerEventsStatus changeLanguage();

    /**
	 * @brief Function for closing app
	 *
	 * @return HandlerEventsStatus: any error is "ERROR_HANDLER", succes is "HANDLED"
	 */

    GlobalHandlerEvents::HandlerEventsStatus quitApp();

    /**
	 * @brief Function for connect via USB
	 *
	 * @return HandlerEventsStatus: any error is "ERROR_HANDLER", succes is "HANDLED"
	 */

    GlobalHandlerEvents::HandlerEventsStatus connect();

    /**
	 * @brief Function for USB disconnect
	 *
	 * @return HandlerEventsStatus: any error is "ERROR_HANDLER", succes is "HANDLED"
	 */

    GlobalHandlerEvents::HandlerEventsStatus disconnect();

	/**
	 * @brief Function for enable/disable sensitive ESP32/STM32 search/download functions
	 *
	 * @return HandlerEventsStatus: any error is "ERROR_HANDLER", succes is "HANDLED"
	 */

    GlobalHandlerEvents::HandlerEventsStatus selectEsp32();

	/**
	 * @brief Function for disable sensitive STM32/ESP32 search/download functions
	 *
	 * @return HandlerEventsStatus: any error is "ERROR_HANDLER", succes is "HANDLED"
	 */

    GlobalHandlerEvents::HandlerEventsStatus selectStm32();

	/**
	 * @brief Function for open search dialog window
	 *
	 * @return HandlerEventsStatus: any error is "ERROR_HANDLER", succes is "HANDLED"
	 */

    GlobalHandlerEvents::HandlerEventsStatus searchFile(HandlersFuncKeys &aKey);

	/**
	 * @brief Function for uploadFirmware
	 *
	 * @return HandlerEventsStatus: any error is "ERROR_HANDLER", succes is "HANDLED"
	 */

    GlobalHandlerEvents::HandlerEventsStatus uploadFirmware();

private:
	/**
	 * @brief Init global handlers
	 *
	 * @return None
	 */

	void initGlobalHandlersEvents();

private:
    GlobalHandlerEvents globalHandlers;
	UsbCommunication usbCommunication;
    Glib::RefPtr<Gtk::Application> perentApp;
    Glib::RefPtr<Gtk::Builder> refBuilder;

private:
	LocusBiaconWidgets::OpenFileDialog *openFileDialog;			///<! Find file for upload
	LocusBiaconWidgets::ConnectionDialog *connectionDialog;		///<! Dialog for USB connection
	LocusBiaconWidgets::MenuBar *menuBar;                       ///<! Menu bar class
	LocusBiaconWidgets::FirmwareMethodBox *fmBox;               ///<! Selection of downloadable firmware
	LocusBiaconWidgets::SelectFirmwareBox *selectFirmwareBox;   ///<! Firmware control

};
#endif // SOURCES_MAINWINDOW_HPP_
