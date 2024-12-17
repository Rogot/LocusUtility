//
// ManuBar.cpp
//
//  Created on: Nov 21, 2024
//  Author: Maksim Sushkov
//

#include "CustomWidgets/MenuBar.hpp"

namespace LocusBiaconWidgets {

MenuBar::MenuBar(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder) : 
    Glib::ObjectBase("MenuBarLB"),
    Gtk::MenuBar(aCobject),
    refBuilder(aBuilder),
    kCurrentItemsText(nullptr),
    kCurrentImageItemsText(nullptr)
{
    #if DEBUG
    std::cout << "Constructor MenuBar" << std::endl;
    #endif
    /*
    * Get MenuItems from .glade
    */
    connectionMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
        aBuilder->get_object("ConnectionMenuItem")
    );
    viewMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
        aBuilder->get_object("ViewMenuItem")
    );
    updateMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
        aBuilder->get_object("UpdateMenuItem")
    );
    helpMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
        aBuilder->get_object("HelpMenuItem")
    );

    /*
    * Get ImageMenuItems from .glade. Connecting
    */
    connectImageMenuItem = Glib::RefPtr<Gtk::ImageMenuItem>::cast_dynamic(
        aBuilder->get_object("ConnectImageMenuItem")
    );

    disconnectImageMenuItem = Glib::RefPtr<Gtk::ImageMenuItem>::cast_dynamic(
        aBuilder->get_object("DisconnectImageMenuItem")
    );

    quitImageMenuItem = Glib::RefPtr<Gtk::ImageMenuItem>::cast_dynamic(
        aBuilder->get_object("QuitImageMenuItem")
    );

    /*
    * View
    */
    languageImageMenuItem = Glib::RefPtr<Gtk::ImageMenuItem>::cast_dynamic(
        aBuilder->get_object("LanguageImageMenuItem")
    );

    themeImageMenuItem = Glib::RefPtr<Gtk::ImageMenuItem>::cast_dynamic(
        aBuilder->get_object("ThemeImageMenuItem")
    );

    /*
    * Update
    */
    searchImageMenuItem = Glib::RefPtr<Gtk::ImageMenuItem>::cast_dynamic(
        aBuilder->get_object("SearchImageMenuItem")
    );

    /*
    * Help
    */
    firmwareImageMenuItem = Glib::RefPtr<Gtk::ImageMenuItem>::cast_dynamic(
        aBuilder->get_object("FirmwareImageMenuItem")
    );

    websiteImageMenuItem = Glib::RefPtr<Gtk::ImageMenuItem>::cast_dynamic(
        aBuilder->get_object("WebsiteImageMenuItem")
    );

    definitionDefaultValues();
}

MenuBar::~MenuBar()
{

}

void MenuBar::definitionDefaultValues()
{
    #if DEBUG
    std::cout << "Definition default values MenuBar" << std::endl;
    #endif
    redefinitionLabeles();
    
    if (connectImageMenuItem) {
        connectImageMenuItem->set_use_underline(true);
        connectImageMenuItem->signal_activate().connect([&]() {
            #if DEBUG_INFO
            std::cout << "Connection/Connect" << std::endl;
            #endif

            GlobalHandlerEvents::HandlerEventsStatus status = globalEvents->executeHandler(
                HandlersFuncKeys::CONNECT
            );
            if (status == GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER) {
                std::cout << "\nError with USB connection!\n";
            } else {
                std::cout << "\nUSB connection success!\n";
            }
        });
    }

    if (disconnectImageMenuItem) {
        disconnectImageMenuItem->set_use_underline(true);
        disconnectImageMenuItem->signal_activate().connect([&]() {
            #if DEBUG_INFO
            std::cout << "Connection/Disconnect" << std::endl;
            #endif

            GlobalHandlerEvents::HandlerEventsStatus status = globalEvents->executeHandler(
                HandlersFuncKeys::DISCONNECT
            );
            if (status == GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER) {
                std::cout << "\nError with USB disconnection!\n";
            } else {
                std::cout << "\nUSB disconnection success!\n";
            }
        });
    }

    if (quitImageMenuItem) {
        quitImageMenuItem->set_use_underline(true);
        quitImageMenuItem->signal_activate().connect([&]() {
            #if DEBUG_INFO
            std::cout << "Connection/Quit" << std::endl;
            #endif

            GlobalHandlerEvents::HandlerEventsStatus status = globalEvents->executeHandler(
                HandlersFuncKeys::QUIT_APP
            );
            if (status == GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER) {
                std::cout << "\nError with closing\n";
            }

        });
    }

    if (languageImageMenuItem) {
        languageImageMenuItem->set_use_underline(true);
        languageImageMenuItem->signal_activate().connect([&]() {
            #if DEBUG_INFO
            std::cout << "Connection/Language" << std::endl;
            #endif

            GlobalHandlerEvents::HandlerEventsStatus status = globalEvents->executeHandler(
                HandlersFuncKeys::SWITCH_LANG
            );
            if (status == GlobalHandlerEvents::HandlerEventsStatus::ERROR_HANDLER) {
                std::cout << "\nError with changing language\n";
            }
        });
    }

    if (themeImageMenuItem) {
        themeImageMenuItem->set_use_underline(true);
        themeImageMenuItem->signal_activate().connect([&]() {
            #if DEBUG_INFO
            std::cout << "Connection/Theme" << std::endl;
            #endif
        });
    }

    if (searchImageMenuItem) {
        searchImageMenuItem->set_use_underline(true);
        searchImageMenuItem->signal_activate().connect([&]() {
            #if DEBUG_INFO
            std::cout << "Connection/Search" << std::endl;
            #endif
        });
    }

    if (firmwareImageMenuItem) {
        firmwareImageMenuItem->set_use_underline(true);
        firmwareImageMenuItem->signal_activate().connect([&]() {
            #if DEBUG_INFO
            std::cout << "Connection/Firmware" << std::endl;
            #endif
        });
    }

    if (websiteImageMenuItem) {
        websiteImageMenuItem->set_use_underline(true);
        websiteImageMenuItem->signal_activate().connect([&]() {
            #if DEBUG_INFO
            std::cout << "Connection/Website" << std::endl;
            #endif
        });
    }
}

void MenuBar::redefinitionLabeles()
{
    #if DEBUG
    std::cout << "Redefinition labeles MenuBar" << std::endl;
    #endif
    // Change Language
    if (kCurrentItemsText != nullptr && kCurrentImageItemsText != nullptr) {
        if (kCurrentItemsText == &kMenuItemsTextRu && kCurrentImageItemsText == &kMenuImageItemsTextRu) {
            kCurrentItemsText = &kMenuItemsTextEn;
            kCurrentImageItemsText = &kMenuImageItemsTextEn;
        } else if (kCurrentItemsText == &kMenuItemsTextEn && kCurrentImageItemsText == &kMenuImageItemsTextEn) {
            kCurrentItemsText = &kMenuItemsTextRu;
            kCurrentImageItemsText = &kMenuImageItemsTextRu;
        }
    } else {
        kCurrentItemsText = &kMenuItemsTextRu;
        kCurrentImageItemsText = &kMenuImageItemsTextRu;
    }

    // Redefinition
    if (connectionMenuItem) {
        connectionMenuItem->set_label(kCurrentItemsText->at(ItemsText::CONNECTION).c_str());
    }
    if (viewMenuItem) {
        viewMenuItem->set_label(kCurrentItemsText->at(ItemsText::VIEW).c_str());
    }
    if (updateMenuItem) {
        updateMenuItem->set_label(kCurrentItemsText->at(ItemsText::UPDATE).c_str());
    }
    if (helpMenuItem) {
        helpMenuItem->set_label(kCurrentItemsText->at(ItemsText::HELP).c_str());
    }

    if (connectImageMenuItem) {
        connectImageMenuItem->set_label(kCurrentImageItemsText->at(ImageItemsText::CONNECT).c_str());
    }

    if (disconnectImageMenuItem) {
        disconnectImageMenuItem->set_label(kCurrentImageItemsText->at(ImageItemsText::DISCONNECT).c_str());
    }
    
    if (quitImageMenuItem) {
        quitImageMenuItem->set_label(kCurrentImageItemsText->at(ImageItemsText::QUIT).c_str());
    }
    
    if (languageImageMenuItem) {
        languageImageMenuItem->set_label(kCurrentImageItemsText->at(ImageItemsText::LANGUAGE).c_str());
    }
    
    if (themeImageMenuItem) {
        themeImageMenuItem->set_label(kCurrentImageItemsText->at(ImageItemsText::THEME).c_str());
    }

    if (searchImageMenuItem) {
        searchImageMenuItem->set_label(kCurrentImageItemsText->at(ImageItemsText::SEARCH_UPDATES).c_str());
    }

    if (firmwareImageMenuItem) {
        firmwareImageMenuItem->set_label(kCurrentImageItemsText->at(ImageItemsText::FIRMWARE).c_str());
    }

    if (websiteImageMenuItem) {
        websiteImageMenuItem->set_label(kCurrentImageItemsText->at(ImageItemsText::WEBSITE).c_str());
    }
}

} // LocusBiaconWidgets