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
    kCirrentFirmwareMethodText{nullptr},
    refBuilder(aBuilder)
{
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

}

void FirmwareMethodBox::definitionDefaultValues()
{
    redefinitionLabeles();

    for (size_t i = 0; i < kDefaultMethodsList.size(); ++i) {
        methodSelector->append(kDefaultMethodsList[i]);
    }
    methodSelector->set_active(1);
}

void FirmwareMethodBox::redefinitionLabeles()
{
    if (kCirrentFirmwareMethodText != nullptr) {
        if (kCirrentFirmwareMethodText == &kFirmwareMethodTextRu) {
            kCirrentFirmwareMethodText = &kFirmwareMethodTextEn;
        } else if (kCirrentFirmwareMethodText == &kFirmwareMethodTextEn) {
            kCirrentFirmwareMethodText = &kFirmwareMethodTextRu;
        }
    } else {
        kCirrentFirmwareMethodText = &kFirmwareMethodTextRu;
    }

    if (methodSelectorLabel) {
        methodSelectorLabel->set_text(kCirrentFirmwareMethodText->at(FirmwareMethodText::BOX_LABEL).c_str());
    }

    if (uploadButton) {
        uploadButton->set_label(kCirrentFirmwareMethodText->at(FirmwareMethodText::UPLOAD_BUTTON).c_str());
    }
}

} // LocusBiacon