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

}

MainWindow::~MainWindow()
{
    if (fmBox != nullptr) {
        delete fmBox;
    }
    if (menuBar != nullptr) {
        delete menuBar;
    }
    if (selectFirmwareBox != nullptr) {
        delete selectFirmwareBox;
    }
    if (dialog != nullptr) {
        delete dialog;
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

    initGlobalHandlersEvents();

    if (aBuilder) {
        aBuilder->get_widget_derived("FirmwareMethodBox", fmBox);
        aBuilder->get_widget_derived("MenuBar", menuBar);
        aBuilder->get_widget_derived("SelectFirmwareBox", selectFirmwareBox);
        
        aBuilder->get_widget_derived("ConnectionDialog", dialog);

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
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::changeLanguage() 
{
    #if DEBUG
    std::cout << "ChangeLanguage" << std::endl;
    #endif
    if (fmBox) { fmBox->redefinitionLabeles(); }
    if (menuBar) { menuBar->redefinitionLabeles(); }
    if (selectFirmwareBox) { selectFirmwareBox->redefinitionLabeles(); }
    if (dialog) { dialog->redefinitionLabeles(); }
    return GlobalHandlerEvents::HandlerEventsStatus::HANDLE;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::quitApp()
{
    #if DEBUG
    std::cout << "QuitApp" << std::endl;
    #endif
    if (perentApp) { perentApp->quit(); }
    return GlobalHandlerEvents::HandlerEventsStatus::HANDLE;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::connect()
{
    #if DEBUG
    std::cout << "Connect USB Device" << std::endl;
    #endif
    if (dialog) { 
        dialog->show();
        dialog->setOwner(this);
        set_sensitive(false);

        return dialog->dialogEnter();
    }

    return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::disconnect()
{
    #if DEBUG
    std::cout << "Disconnect USB Device" << std::endl;
    #endif
    if (dialog) { 
        return dialog->disconnect();
    }
    return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::selectEsp32()
{
    #if DEBUG
    std::cout << "Off sensitive for search/download STM32 and On for ESP32" << std::endl;
    #endif
    if (selectFirmwareBox) {
        selectFirmwareBox->stm32Inactive();
        selectFirmwareBox->esp32Active();
        return GlobalHandlerEvents::HandlerEventsStatus::HANDLE;
    }

    return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::selectStm32()
{
    #if DEBUG
    std::cout << "Off sensitive for search/download ESP32 and On for STM32" << std::endl;
    #endif
    if (selectFirmwareBox) {
        selectFirmwareBox->esp32Inactive();
        selectFirmwareBox->stm32Active();
        return GlobalHandlerEvents::HandlerEventsStatus::HANDLE;
    }

    return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
}