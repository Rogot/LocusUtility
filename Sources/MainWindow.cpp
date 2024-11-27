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

}

MainWindow *MainWindow::getInstance(const char* aTitle, Glib::RefPtr<Gtk::Application> &aApp)
{
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
    perentApp = aApp;
    if (aBuilder) {
        aBuilder->get_widget_derived("FirmwareMethodBox", fmBox);
        aBuilder->get_widget_derived("MenuBar", menuBar);
        aBuilder->get_widget_derived("SelectFirmwareBox", selectFirmwareBox);
        menuBar->setGlobalHandlerEvents(globalHandlers);
    }

    globalHandlers.addHandler("ChangeLanguage", [this]() {
        return this->changeLanguage();
    });

    globalHandlers.addHandler("QuitApp", [this]() {
       return this->quitApp();
    });

    globalHandlers.addHandler("Connect", [this]() {
       return this->connect();
    });
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::changeLanguage() 
{
    #if DEBUG
    std::cout << "ChangeLanguage" << std::endl;
    #endif
    fmBox->redefinitionLabeles();
    menuBar->redefinitionLabeles();
    selectFirmwareBox->redefinitionLabeles();
    return GlobalHandlerEvents::HandlerEventsStatus::HANDLE;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::quitApp()
{
    #if DEBUG
    std::cout << "QuitApp" << std::endl;
    #endif
    perentApp->quit();
    return GlobalHandlerEvents::HandlerEventsStatus::HANDLE;
}

GlobalHandlerEvents::HandlerEventsStatus MainWindow::connect()
{
    #if DEBUG
    std::cout << "Connect USB Device" << std::endl;
    #endif
    
    if (!usb.getDeviceList()) {
        return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
    }
    
    if (!usb.connectToDevice()) {
        return GlobalHandlerEvents::HandlerEventsStatus::ERROR;
    }

    return GlobalHandlerEvents::HandlerEventsStatus::HANDLE;
}