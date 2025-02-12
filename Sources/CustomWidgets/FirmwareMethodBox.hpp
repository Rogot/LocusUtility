//
// FirmwareMethodComboBox.hpp
//
//  Created on: Nov 19, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CUSTOMWIDGETS_FIRMWAREMETHODBOX_HPP_
#define SOURCES_CUSTOMWIDGETS_FIRMWAREMETHODBOX_HPP_

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <gtkmm-3.0/gtkmm/box.h>
#include <gtkmm-3.0/gtkmm/builder.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/comboboxtext.h>
#include <gtkmm-3.0/gtkmm/label.h>

#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"
#include "CustomWidgets/OpenFileDialog.hpp"

#define DEBUG       false

namespace LocusBiaconWidgets {

class FirmwareMethodBox : public Gtk::Box {
private:
    enum class FirmwareMethodText {
        BOX_LABEL,
        UPLOAD_BUTTON
    };

    const std::map<FirmwareMethodText, std::string> kFirmwareMethodTextRu = {
        {FirmwareMethodText::BOX_LABEL, "Выбор способа прошивки"},
        {FirmwareMethodText::UPLOAD_BUTTON, "Загрузить"},
    };

    const std::map<FirmwareMethodText, std::string> kFirmwareMethodTextEn = {
        {FirmwareMethodText::BOX_LABEL, "Choosing the firmware method"},
        {FirmwareMethodText::UPLOAD_BUTTON, "Upload"},
    };

    const std::map<FirmwareMethodText, std::string>  *kCurrentFirmwareMethodText;

    const std::vector<std::string> kDefaultMethodsList = {
        "STM32",
        "ESP32"
    };

public:
    FirmwareMethodBox(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder);

    virtual ~FirmwareMethodBox();

    /**
	 * @brief Update labels when change the language
	 *
	 * @return None
	 */

    void redefinitionLabeles();

    /**
	 * @brief Set global handler event ptr
	 *
     * @param [aGlobalEvents] Global handler event object
	 * @return None
	 */

    void setGlobalHandlerEvents(GlobalHandlerEvents &aGlobalEvents) { globalEvents = &aGlobalEvents; }
    
    /**
	 * @brief Set device for download firmware
	 *
	 * @return None
	 */

    void setActiveMethod();

    /**
	 * @brief Upload frimware
	 *
	 * @return None
	 */

    void upload();

protected:
    /**
	 * @brief Set standard values for an object of the FirmwareMethodComboBox class
	 *
	 * @return None
	 */
    void definitionDefaultValues();

private:
    Glib::RefPtr<Gtk::Builder> refBuilder;

    Glib::RefPtr<Gtk::ComboBoxText> methodSelector; ///<! Selector for downloadable firmware method
    Glib::RefPtr<Gtk::Label> methodSelectorLabel;   ///<! Lable with description of block

    Glib::RefPtr<Gtk::Button> uploadButton;          ///<! Lable with description of block

private:
    LocusBiaconWidgets::OpenFileDialog *openFileDialog;
    GlobalHandlerEvents *globalEvents;
};

} // LocusBiacon

#endif // SOURCES_CUSTOMWIDGETS_FIRMWAREMETHODBOX_HPP_