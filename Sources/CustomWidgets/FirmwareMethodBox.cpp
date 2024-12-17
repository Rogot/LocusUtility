//
// FirmwareMethodComboBox.cpp
//
//  Created on: Nov 19, 2024
//  Author: Maksim Sushkov
//

#include "CustomWidgets/FirmwareMethodBox.hpp"

namespace LocusBiaconWidgets {

FirmwareMethodBox::FirmwareMethodBox(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder) :
    Glib::ObjectBase("FMBoxLB"),
    Gtk::Box(aCobject),
    kCurrentFirmwareMethodText{nullptr},
    openFileDialog(nullptr),
    refBuilder(aBuilder)
{
    #if DEBUG
    std::cout << "Constructor FirmwareMethodBox" << std::endl;
    #endif

    methodSelector = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
        aBuilder->get_object("FirmwareMethodComboBoxText")
    );

    methodSelectorLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        aBuilder->get_object("FirmwareMethodLable")
    );

    uploadButton = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        aBuilder->get_object("UploadButton")
    ); 

    definitionDefaultValues();
}

FirmwareMethodBox::~FirmwareMethodBox()
{
    if (openFileDialog) {
        delete openFileDialog;
    }
}

void FirmwareMethodBox::definitionDefaultValues()
{
    #if DEBUG
    std::cout << "Definition default values FirmwareMethodBox" << std::endl;
    #endif
    redefinitionLabeles();

    if (methodSelector) {
        for (size_t i = 0; i < kDefaultMethodsList.size(); ++i) {
            methodSelector->append(kDefaultMethodsList[i]);
        }
        methodSelector->set_active(0);

        methodSelector->signal_changed().connect([&]() {
            if (kDefaultMethodsList[0] == methodSelector->get_active_text()) {
                globalEvents->executeHandler(HandlersFuncKeys::SELECT_STM32);
            } else if (kDefaultMethodsList[1] == methodSelector->get_active_text()) {
                globalEvents->executeHandler(HandlersFuncKeys::SELECT_ESP32);
            }
        });

        uploadButton->signal_clicked().connect([&]() {
            
        });
    }
}

void FirmwareMethodBox::redefinitionLabeles()
{
    #if DEBUG
    std::cout << "Redefinition labeles FirmwareMethodBox" << std::endl;
    #endif
    if (kCurrentFirmwareMethodText != nullptr) {
        if (kCurrentFirmwareMethodText == &kFirmwareMethodTextRu) {
            kCurrentFirmwareMethodText = &kFirmwareMethodTextEn;
        } else if (kCurrentFirmwareMethodText == &kFirmwareMethodTextEn) {
            kCurrentFirmwareMethodText = &kFirmwareMethodTextRu;
        }
    } else {
        kCurrentFirmwareMethodText = &kFirmwareMethodTextRu;
    }

    if (methodSelectorLabel) {
        methodSelectorLabel->set_text(kCurrentFirmwareMethodText->at(FirmwareMethodText::BOX_LABEL).c_str());
    }

    if (uploadButton) {
        uploadButton->set_label(kCurrentFirmwareMethodText->at(FirmwareMethodText::UPLOAD_BUTTON).c_str());
    }
}

} // LocusBiacon