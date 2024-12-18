//
// OpenFileDialog.hpp
//
//  Created on: Dec 17, 2024
//  Author: Maksim Sushkov
//

#ifndef SOURCES_CUSTOMWIDGETS_OPENFILEDIALOG_HPP_
#define SOURCES_CUSTOMWIDGETS_OPENFILEDIALOG_HPP_

#include <map>

#include <gtkmm-3.0/gtkmm/builder.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/filechooserdialog.h>
#include <gtkmm-3.0/gtkmm/filefilter.h>

#include "Core/GlobalHandlerEvents/GlobalHandlerEvents.hpp"

#define DEBUG       false

namespace LocusBiaconWidgets {

class OpenFileDialog : public Gtk::FileChooserDialog {
private:
    enum class OpenFileDialogText {
        CANCEL,
        OPEN
    };

    const std::map<OpenFileDialogText, std::string> kOpenFileDialogTextRu = {
        {OpenFileDialogText::CANCEL, "Отмена"},
        {OpenFileDialogText::OPEN, "Открыть"}
    };

    const std::map<OpenFileDialogText, std::string> kOpenFileDialogTextEn = {
        {OpenFileDialogText::CANCEL, "Cancel"},
        {OpenFileDialogText::OPEN, "Open"}
    };

    const std::map<OpenFileDialogText, std::string> *kOpenFileDialogText;

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

    /**
	 * @brief Initial function
	 *
	 * @return None
	 */

    void init(std::vector<std::string> &aFilesTypes);

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

    Glib::RefPtr<Gtk::FileFilter> fileFilter;
    Gtk::Button *openButton;
    Gtk::Button *cencelButton;
    // Glib::RefPtr<Gtk::Button> openButton;
    // Glib::RefPtr<Gtk::Button> cencelButton;

private:
    Gtk::Window *ownerWindow;
    GlobalHandlerEvents *globalEvents;
};

} // LocusBiaconWidgets

#endif // SOURCES_CUSTOMWIDGETS_OPENFILEDIALOG_HPP_