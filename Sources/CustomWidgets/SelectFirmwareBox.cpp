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

    definitionDefaultValues();
}

SelectFirmwareBox::~SelectFirmwareBox()
{

}

void SelectFirmwareBox::redefinitionLabeles()
{
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
}

void SelectFirmwareBox::definitionDefaultValues()
{
    redefinitionLabeles();
}

} // LocusBiaconWidgets