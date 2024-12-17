//
// OpenFileDialog.hpp
//
//  Created on: Dec 17, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CUSTOMWIDGETS_OPENFILEDIALOG_HPP_
#define SOURCES_CUSTOMWIDGETS_OPENFILEDIALOG_HPP_

#include <gtkmm-3.0/gtkmm/builder.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/filechooserdialog.h>
#include <gtkmm-3.0/gtkmm/filefilter.h>

#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"

#define DEBUG       false

namespace LocusBiaconWidgets {

class OpenFileDialog : public Gtk::FileChooserDialog {
private:

public:
    OpenFileDialog(BaseObjectType* aCobject, const Glib::RefPtr<Gtk::Builder>& aBuilder);

    virtual ~OpenFileDialog();

    /**
	 * @brief Update labels
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

    void setOwner(Gtk::Window *aOwner);

protected:
    /**
	 * @brief Set standard values for an object of the SelectFirmwareBox class
	 *
	 * @return None
	 */

    void definitionDefaultValues();

private:
    Glib::RefPtr<Gtk::Builder> refBuilder;

    Glib::RefPtr<Gtk::Button> cancelButton;
    Glib::RefPtr<Gtk::Button> openButton;
    Glib::RefPtr<Gtk::FileFilter> fileFilter;

private:
    Gtk::Window *ownerWindow;
    GlobalHandlerEvents *globalEvents;
};

} // LocusBiaconWidgets

#endif // SOURCES_CUSTOMWIDGETS_OPENFILEDIALOG_HPP_