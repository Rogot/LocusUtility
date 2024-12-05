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
    if (aBuilder) {
        aBuilder->get_widget_derived("FirmwareMethodBox", fmBox);
        aBuilder->get_widget_derived("MenuBar", menuBar);
        aBuilder->get_widget_derived("SelectFirmwareBox", selectFirmwareBox);
        
        aBuilder->get_widget_derived("ConnectionDialog", dialog);

        if (menuBar) {
            menuBar->setGlobalHandlerEvents(globalHandlers);
        }
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
    dialog->redefinitionLabeles();
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

    dialog->show();
    dialog->setOwner(this);
    set_sensitive(false);

    return dialog->dialogHundler();
}