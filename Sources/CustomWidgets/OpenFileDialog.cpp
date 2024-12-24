//
// OpenFileDialog.cpp
//
//  Created on: Dec 17, 2024
//  Author: Maksim Sushkov
//

#include "CustomWidgets/OpenFileDialog.hpp"

namespace LocusBiaconWidgets {

OpenFileDialog::OpenFileDialog(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder) : 
    Glib::ObjectBase("OpenFileDialog"),
    Gtk::FileChooserDialog(aCobject),
    openButton(nullptr),
    cencelButton(nullptr),
    kOpenFileDialogText(nullptr),
    refBuilder(aBuilder)
{
    definitionDefaultValues();
}

OpenFileDialog::~OpenFileDialog()
{
    if (openButton) {
        delete openButton;
    }
    if (cencelButton) {
        delete cencelButton;
    }
}

void OpenFileDialog::definitionDefaultValues()
{
    redefinitionLabeles();
    openButton = add_button(kOpenFileDialogText->at(OpenFileDialogText::OPEN).c_str(), Gtk::RESPONSE_OK);
    cencelButton = add_button(kOpenFileDialogText->at(OpenFileDialogText::CANCEL).c_str(), Gtk::RESPONSE_CANCEL);
    set_current_folder(Glib::ustring::compose("%1/Desktop", Glib::ustring(getenv("HOME"))));
    set_transient_for(*this);
}

void OpenFileDialog::redefinitionLabeles()
{
    #if DEBUG
    std::cout << "Redefinition labeles OpenFileDialog" << std::endl;
    #endif

    if (kOpenFileDialogText != nullptr) {
        if (kOpenFileDialogText == &kOpenFileDialogTextRu) {
            kOpenFileDialogText = &kOpenFileDialogTextEn;
        } else if (kOpenFileDialogText == &kOpenFileDialogTextEn) {
            kOpenFileDialogText = &kOpenFileDialogTextRu;
        }
    } else {
        kOpenFileDialogText = &kOpenFileDialogTextRu;
    }

    if (openButton) {
        openButton->set_label(kOpenFileDialogText->at(OpenFileDialogText::OPEN).c_str());
    }

    if (cencelButton) {
        cencelButton->set_label(kOpenFileDialogText->at(OpenFileDialogText::CANCEL).c_str());
    }

}

void OpenFileDialog::setOwner(Gtk::Window *aOwner)
{
    ownerWindow = aOwner;
}

void OpenFileDialog::init(std::vector<std::string> &aFilesTypes)
{
    for (size_t i = 0; i < aFilesTypes.size(); i += 2) {
        fileFilter = Gtk::FileFilter::create();
        fileFilter->set_name(aFilesTypes[i] + " Files (" + aFilesTypes[i + 1] + ")");
        fileFilter->add_pattern(aFilesTypes[i + 1]);
        add_filter(fileFilter);
    }
}

}