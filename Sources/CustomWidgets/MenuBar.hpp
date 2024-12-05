//
// ManuBar.hpp
//
//  Created on: Nov 21, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CUSTOMWIDGETS_MENUBAR_HPP_
#define SOURCES_CUSTOMWIDGETS_MENUBAR_HPP_

#include <iostream>
#include <map>

#include <gtkmm-3.0/gtkmm/builder.h>
#include <gtkmm-3.0/gtkmm/menubar.h>
#include <gtkmm-3.0/gtkmm/menuitem.h>
#include <gtkmm-3.0/gtkmm/messagedialog.h>
#include <gtkmm-3.0/gtkmm/imagemenuitem.h>

#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"

#define DEBUG           false
#define DEBUG_INFO      true

namespace LocusBiaconWidgets {

class MenuBar : public Gtk::MenuBar {
private:
    enum class ItemsText {
        CONNECTION,
        VIEW,
        UPDATE,
        HELP
    };

    enum class ImageItemsText {
        //Connection
        CONNECT,
        DISCONNECT,
        QUIT,
        //View
        LANGUAGE,
        THEME,
        //Update
        SEARCH_UPDATES,
        //Help
        FIRMWARE,
        WEBSITE
    };

    const std::map<ItemsText, std::string> kMenuItemsTextRu = {
        {ItemsText::CONNECTION, "Подключение"},
        {ItemsText::VIEW, "Вид"},
        {ItemsText::UPDATE, "Обновление"},
        {ItemsText::HELP, "Помощь"},
    };

    const std::map<ImageItemsText, std::string> kMenuImageItemsTextRu = {
        {ImageItemsText::CONNECT, "Подключиться"},
        {ImageItemsText::DISCONNECT, "Отключиться"},
        {ImageItemsText::QUIT, "Выход"},
        {ImageItemsText::LANGUAGE, "Язык"},
        {ImageItemsText::THEME, "Тема"},
        {ImageItemsText::SEARCH_UPDATES, "Поиск обновлений"},
        {ImageItemsText::FIRMWARE, "Сайт с прошивками"},
        {ImageItemsText::WEBSITE, "Сайт компании"}
    };

    const std::map<ItemsText, std::string> kMenuItemsTextEn = {
        {ItemsText::CONNECTION, "Connection"},
        {ItemsText::VIEW, "View"},
        {ItemsText::UPDATE, "Update"},
        {ItemsText::HELP, "Help"},
    }; 

    const std::map<ImageItemsText, std::string> kMenuImageItemsTextEn = {
        {ImageItemsText::CONNECT, "Connect"},
        {ImageItemsText::DISCONNECT, "Disconnect"},
        {ImageItemsText::QUIT, "Quit"},
        {ImageItemsText::LANGUAGE, "Language"},
        {ImageItemsText::THEME, "Theme"},
        {ImageItemsText::SEARCH_UPDATES, "Search Updates"},
        {ImageItemsText::FIRMWARE, "Website with firmware"},
        {ImageItemsText::WEBSITE, "Company's website"}
    };

    const std::map<ItemsText, std::string> *kCurrentItemsText;              ///<! Using for switch lenguage
    const std::map<ImageItemsText, std::string> *kCurrentImageItemsText;    ///<! Using for switch lenguage


public:
    MenuBar(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder);

    virtual ~MenuBar();

    /**
	 * @brief Set global handler event ptr
	 *
     * @param [aGlobalEvents] Global handler event object
	 * @return None
	 */
    void setGlobalHandlerEvents(GlobalHandlerEvents &aGlobalEvents) { globalEvents = &aGlobalEvents; }


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
    Glib::RefPtr<Gtk::MenuItem> connectionMenuItem;             ///<! Connection menu-selector
    Glib::RefPtr<Gtk::MenuItem> viewMenuItem;                   ///<! View menu-selector
    Glib::RefPtr<Gtk::MenuItem> updateMenuItem;                 ///<! Update menu-selector
    Glib::RefPtr<Gtk::MenuItem> helpMenuItem;                   ///<! Help menu-selector

    Glib::RefPtr<Gtk::ImageMenuItem> connectImageMenuItem;      ///<! Make connection
    Glib::RefPtr<Gtk::ImageMenuItem> disconnectImageMenuItem;   ///<! Make disconnection
    Glib::RefPtr<Gtk::ImageMenuItem> quitImageMenuItem;         ///<! Make quit
    Glib::RefPtr<Gtk::ImageMenuItem> languageImageMenuItem;     ///<! Change language
    Glib::RefPtr<Gtk::ImageMenuItem> themeImageMenuItem;        ///<! Change theme
    Glib::RefPtr<Gtk::ImageMenuItem> searchImageMenuItem;       ///<! Search updates
    Glib::RefPtr<Gtk::ImageMenuItem> firmwareImageMenuItem;     ///<! Site with firmware
    Glib::RefPtr<Gtk::ImageMenuItem> websiteImageMenuItem;      ///<! Company's website

private:
    Glib::RefPtr<Gtk::Builder> refBuilder;
    GlobalHandlerEvents *globalEvents;
};

} // LocusBiaconWidgets

#endif // SOURCES_CUSTOMWIDGETS_MENUBAR_HPP_