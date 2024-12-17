//
// SelectFirmwareBox.cpp
//
//  Created on: Nov 25, 2024
//  Author: Maksim Sushkov
//

#include "CustomWidgets/SelectFirmwareBox.hpp"

namespace LocusBiaconWidgets {

SelectFirmwareBox::SelectFirmwareBox(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder) :
    Glib::ObjectBase("SFBoxLB"),
    Gtk::Box(aCobject),
    kCurrentSearchFirmwareText(nullptr),
    refBuilder(aBuilder)
{
    #if DEBUG
    std::cout << "Constructor SelectFirmwareBox" << std::endl;
    #endif

    selectFirmwareEsp32Label = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("SelectFirmwareBoxEsp32Label")
    );

    selectFirmwareStm32Label = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("SelectFirmwareBoxStm32Label")
    );

    selectFirmwareEsp32Button = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("SelectFirmwareEsp32Button")
    );

    selectFirmwareStm32Button = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("SelectFirmwareStm32Button")
    );

    selectFirmwareEsp32SearchEntry = Glib::RefPtr<Gtk::SearchEntry>::cast_dynamic(
        aBuilder->get_object("SelectFirmwareSearchEntryEsp32")
    );

    selectFirmwareStm32SearchEntry = Glib::RefPtr<Gtk::SearchEntry>::cast_dynamic(
        aBuilder->get_object("SelectFirmwareSearchEntryStm32")
    );

    selectFirmwareLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("SelectFirmwareLabel")
    ); 

    definitionDefaultValues();
}

SelectFirmwareBox::~SelectFirmwareBox()
{

}

void SelectFirmwareBox::redefinitionLabeles()
{
    #if DEBUG
    std::cout << "Redefinition labeles SelectFirmwareBox" << std::endl;
    #endif
    
    if (kCurrentSearchFirmwareText != nullptr) {
        if (kCurrentSearchFirmwareText == &kSearchFirmwareTextRu) {
            kCurrentSearchFirmwareText = &kSearchFirmwareTextEn;
        } else if (kCurrentSearchFirmwareText == &kSearchFirmwareTextEn) {
            kCurrentSearchFirmwareText = &kSearchFirmwareTextRu;
        }       
    } else {
        kCurrentSearchFirmwareText = &kSearchFirmwareTextRu;
    }

    if (selectFirmwareEsp32Label) {
        selectFirmwareEsp32Label->set_label(kCurrentSearchFirmwareText->at(SelectFirmwareText::ESP32_LABEL).c_str());
    }

    if (selectFirmwareStm32Label) {
        selectFirmwareStm32Label->set_label(kCurrentSearchFirmwareText->at(SelectFirmwareText::STM32_LABEL).c_str());
    }

    if (selectFirmwareEsp32Button) {
        selectFirmwareEsp32Button->set_label(kCurrentSearchFirmwareText->at(SelectFirmwareText::SEARCH_BUTTON).c_str());
    }

    if (selectFirmwareStm32Button) {
        selectFirmwareStm32Button->set_label(kCurrentSearchFirmwareText->at(SelectFirmwareText::SEARCH_BUTTON).c_str());
    }

    if (selectFirmwareLabel) {
        selectFirmwareLabel->set_label(kCurrentSearchFirmwareText->at(SelectFirmwareText::MAIN_LABEL).c_str());
    }
}

void SelectFirmwareBox::definitionDefaultValues()
{
    #if DEBUG
    std::cout << "Definition default values SelectFirmwareBox" << std::endl;
    #endif
    redefinitionLabeles();
}

void SelectFirmwareBox::esp32Active()
{
    selectFirmwareEsp32Button->set_sensitive(true);
    selectFirmwareEsp32SearchEntry->set_sensitive(true);
}

void SelectFirmwareBox::esp32Inactive()
{
    selectFirmwareEsp32Button->set_sensitive(false);
    selectFirmwareEsp32SearchEntry->set_sensitive(false);
}

void SelectFirmwareBox::stm32Active()
{
    selectFirmwareStm32Button->set_sensitive(true);
    selectFirmwareStm32SearchEntry->set_sensitive(true);
}

void SelectFirmwareBox::stm32Inactive()
{
    selectFirmwareStm32Button->set_sensitive(false);
    selectFirmwareStm32SearchEntry->set_sensitive(false);
}

} // LocusBiaconWidgets