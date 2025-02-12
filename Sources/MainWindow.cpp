//
// MainWindow.cpp
//
//  Created on: Nov 19, 2024
//  Author: Maksim Sushkov
//

#include "MainWindow.hpp"

MainWindow::MainWindow(BaseObjectType *aCobject, const Glib::RefPtr<Gtk::Builder> &aBuilder) :
    Gtk::ApplicationWindow(aCobject),
    refBuilder(aBuilder)
{
    // Load survey and clean up data
}

MainWindow::~MainWindow()
{
    if (fmBox) {
        delete fmBox;
    }
    if (menuBar) {
        delete menuBar;
    }
    if (selectFirmwareBox) {
        delete selectFirmwareBox;
    }
    if (connectionDialog) {
        delete connectionDialog;
    }
    if (openFileDialog) {
        delete openFileDialog;
    }
}

MainWindow *MainWindow::getInstance(const char* aTitle, Glib::RefPtr<Gtk::Application> &aApp)
{
    #if DEBUG
    std::cout << "getInstance" << std::endl;
    #endif
    MainWindow *result;
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(kUiPath);
    builder->get_widget_derived("MainWindow", result);

    result->initWidgets(builder, aApp);
    
    result->set_title(aTitle);

    result->set_default_size(800,600);

    return result;
}

void MainWindow::initWidgets(Glib::RefPtr<Gtk::Builder> &aBuilder, Glib::RefPtr<Gtk::Application> &aApp)
{
    #if DEBUG
    std::cout << "Init Widgets" << std::endl;
    #endif
    perentApp = aApp;
    refBuilder = aBuilder;

    initGlobalHandlersEvents();

    if (aBuilder) {
        aBuilder->get_widget_derived("FirmwareMethodBox", fmBox);
        aBuilder->get_widget_derived("MenuBar", menuBar);
        aBuilder->get_widget_derived("SelectFirmwareBox", selectFirmwareBox);
        aBuilder->get_widget_derived("ConnectionDialog", connectionDialog);
        aBuilder->get_widget_derived("OpenFileDialog", openFileDialog);

        if (menuBar) {
            menuBar->setGlobalHandlerEvents(globalHandlers);
        }
        if (fmBox) {
            fmBox->setGlobalHandlerEvents(globalHandlers);
        }
        if (selectFirmwareBox) {
            selectFirmwareBox->setGlobalHandlerEvents(globalHandlers);
            globalHandlers.executeHandler(HandlersFuncKeys::SELECT_STM32);
        }
        if (openFileDialog) {
            openFileDialog->setOwner(this);
            
            // Determining the types of files that will be shown during the search (name + type)
            std::vector<std::string> types = { "Binar" ,"*.bin",
                                                "Hex" ,"*.hex",
                                                "All" ,"*.*"};
            openFileDialog->init(types);
        }
    }
}

void MainWindow::initGlobalHandlersEvents()
{
    globalHandlers.addHandler(HandlersFuncKeys::SWITCH_LANG, [this]() {
        return this->changeLanguage();
    });

    globalHandlers.addHandler(HandlersFuncKeys::QUIT_APP, [this]() {
        return this->quitApp();
    });

    globalHandlers.addHandler(HandlersFuncKeys::CONNECT, [this]() {
        return this->connect();
    });

    globalHandlers.addHandler(HandlersFuncKeys::DISCONNECT, [this]() {
        return this->disconnect();
    });

    globalHandlers.addHandler(HandlersFuncKeys::SELECT_STM32, [this]() {
        return this->selectStm32();
    });

    globalHandlers.addHandler(HandlersFuncKeys::SELECT_ESP32, [this]() {
        return this->selectEsp32();
    });

    globalHandlers.addHandler(HandlersFuncKeys::SEARCH_FILE_STM32, [this]() {
        HandlersFuncKeys handleKey = HandlersFuncKeys::SEARCH_FILE_STM32;
        return this->searchFile(handleKey);
    });

    globalHandlers.addHandler(HandlersFuncKeys::SEARCH_FILE_ESP32, [this]() {
        HandlersFuncKeys handleKey = HandlersFuncKeys::SEARCH_FILE_ESP32;
        return this->searchFile(handleKey);
    });

    globalHandlers.addHandler(HandlersFuncKeys::UPLOAD_FIRMWARE, [this]() {
        return this->uploadFirmware();
    });
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::changeLanguage() 
{
    #if DEBUG
    std::cout << "ChangeLanguage" << std::endl;
    #endif
    if (fmBox) { fmBox->redefinitionLabeles(); }
    if (menuBar) { menuBar->redefinitionLabeles(); }
    if (selectFirmwareBox) { selectFirmwareBox->redefinitionLabeles(); }
    if (connectionDialog) { connectionDialog->redefinitionLabeles(); }
    if (openFileDialog) { openFileDialog->redefinitionLabeles(); }

    return GlobalHandlerEvents::HandlerEventsStatus::HANDLED;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::quitApp()
{
    #if DEBUG
    std::cout << "QuitApp" << std::endl;
    #endif
    if (perentApp) { perentApp->quit(); }
    return GlobalHandlerEvents::HandlerEventsStatus::HANDLED;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::connect()
{
    #if DEBUG
    std::cout << "Connect USB Device" << std::endl;
    #endif
    if (connectionDialog) { 
        connectionDialog->setUsbConnetctionPtr(usbCommunication);
        connectionDialog->setOwner(this);
        connectionDialog->show();

        set_sensitive(false);

        return GlobalHandlerEvents::HandlerEventsStatus::HANDLED;
    }

    return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::disconnect()
{
    #if DEBUG
    std::cout << "Disconnect USB Device" << std::endl;
    #endif
    if (connectionDialog) { 
        return connectionDialog->disconnect();
    }
    return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::selectEsp32()
{
    #if DEBUG
    std::cout << "Off sensitive for search/download STM32 and On for ESP32" << std::endl;
    #endif
    if (selectFirmwareBox) {
        selectFirmwareBox->stm32Inactive();
        selectFirmwareBox->esp32Active();
        return GlobalHandlerEvents::HandlerEventsStatus::HANDLED;
    }

    return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::selectStm32()
{
    #if DEBUG
    std::cout << "Off sensitive for search/download ESP32 and On for STM32" << std::endl;
    #endif
    if (selectFirmwareBox) {
        selectFirmwareBox->esp32Inactive();
        selectFirmwareBox->stm32Active();
        return GlobalHandlerEvents::HandlerEventsStatus::HANDLED;
    }

    return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::searchFile(HandlersFuncKeys &aKey)
{
    #if DEBUG
    std::cout << "Start searching file" << std::endl;
    #endif
    
    if (openFileDialog->run() == Gtk::RESPONSE_OK) {
        #if DEBUG
        std::cout << "Open File Dialog is open!!!" << std::endl;
        #endif
    }

    std::string path = openFileDialog->get_filename();
    if (path.size() == 0) {
        openFileDialog->hide();
        return GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER;
    }

    selectFirmwareBox->setFilePath(path, aKey);
    
    openFileDialog->hide();

    return GlobalHandlerEvents::HandlerEventsStatus::HANDLED;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::uploadFirmware()
{
    usbCommunication.initPayloadProtocol();
}