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
    refBuilder(aBuilder)
{
    add_button("Cancel", Gtk::RESPONSE_CANCEL);
    add_button("Open", Gtk::RESPONSE_OK);
    set_current_folder(Glib::ustring::compose("%1/Desktop", Glib::ustring(getenv("HOME"))));
    
    // set_transient_for(*this);

    // fileFilter = Gtk::FileFilter::create();
    // fileFilter->set_name("Text Files (*.txt)");
    // fileFilter->add_pattern("*.txt");
    // add_filter(fileFilter);
    // fileFilter = Gtk::FileFilter::create();
    // fileFilter->set_name("All Files (*.*)");
    // fileFilter->add_pattern("*.*");
    // add_filter(fileFilter);
    
    // if (run() == Gtk::RESPONSE_OK) {
    //     std::cout << "Open File Dialog is open!!!" << std::endl;
    // }
    //   label.set_text(ustring::compose("File = %1", ustring(openFileDialog.get_filename())));
}

OpenFileDialog::~OpenFileDialog()
{

}

void OpenFileDialog::setOwner(Gtk::Window *aOwner)
{
    ownerWindow = aOwner;
}


}