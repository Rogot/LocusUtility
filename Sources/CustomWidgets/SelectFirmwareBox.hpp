//
// SelectFirmwareBox.hpp
//
//  Created on: Nov 25, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CUSTOMWIDGETS_SELECTFIRMWAREBOX_HPP_
#define SOURCES_CUSTOMWIDGETS_SELECTFIRMWAREBOX_HPP_

#include <map>
#include <string>
#include <iostream>

#include <gtkmm-3.0/gtkmm/box.h>
#include <gtkmm-3.0/gtkmm/builder.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/label.h>
#include <gtkmm-3.0/gtkmm/searchentry.h>

#define DEBUG       false

namespace LocusBiaconWidgets {

class SelectFirmwareBox : public Gtk::Box {
    enum class SelectFirmwareText {
        ESP32_LABEL,
        STM32_LABEL,
        SEARCH_BUTTON,
        MAIN_LABEL
    };
    
    const std::map<SelectFirmwareText, std::string> kSearchFirmwareTextRu = {
        {SelectFirmwareText::ESP32_LABEL, "Путь для\nпрошивки ESP32"},
        {SelectFirmwareText::STM32_LABEL, "Путь для\nпрошивки STM32"},
        {SelectFirmwareText::SEARCH_BUTTON, "Поиск"},
        {SelectFirmwareText::MAIN_LABEL, "Загрузка прошивки"}
    };

    const std::map<SelectFirmwareText, std::string> kSearchFirmwareTextEn = {
        {SelectFirmwareText::ESP32_LABEL, "Firmware\npath for ESP32"},
        {SelectFirmwareText::STM32_LABEL, "Firmware\npath for STM32"},
        {SelectFirmwareText::SEARCH_BUTTON, "Search"},
        {SelectFirmwareText::MAIN_LABEL, "Upload firmware"}
    };

    const std::map<SelectFirmwareText, std::string> *kCurrentSearchFirmwareText;

public:
    SelectFirmwareBox(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder);

    virtual ~SelectFirmwareBox();

    /**
	 * @brief Update labels
	 *
	 * @return None
	 */

    void redefinitionLabeles();

protected:
    /**
	 * @brief Set standard values for an object of the SelectFirmwareBox class
	 *
	 * @return None
	 */

    void definitionDefaultValues();

private:
    Glib::RefPtr<Gtk::Builder> refBuilder;

    Glib::RefPtr<Gtk::Label> selectFirmwareEsp32Label;
    Glib::RefPtr<Gtk::Button> selectFirmwareEsp32Button;            ///<! Search firmware for ESP32 in OS (search using UI)
    Glib::RefPtr<Gtk::SearchEntry> selectFirmwareEsp32SearchEntry;  ///<! Search firmware for ESP32 in OS (write a path)

    Glib::RefPtr<Gtk::Label> selectFirmwareStm32Label;
    Glib::RefPtr<Gtk::Button> selectFirmwareStm32Button;            ///<! Search firmware for STM32 in OS (search using UI)
    Glib::RefPtr<Gtk::SearchEntry> selectFirmwareStm32SearchEntry;  ///<! Search firmware for STM32 in OS (write a path)

    Glib::RefPtr<Gtk::Label> selectFirmwareLabel;
};

} // LocusBiaconWidgets

#endif // SOURCES_CUSTOMWIDGETS_SELECTFIRMWAREBOX_HPP_