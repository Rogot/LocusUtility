//
// main.cpp
//
//  Created on: Nov 19, 2024
//  Author: Maksim Sushkov
//

#include "main.hpp"
#include <DroneDevice/Libs/libcanard/canard.h>
// #include <DroneDevice/PayloadProtocol/SerialHandler.hpp>
// #include <DroneDevice/Include/DroneDevice/AbstractDevice.hpp>

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> application = Gtk::Application::create(argc, argv);

    try {
        MainWindow *mainWindow = MainWindow::getInstance("LocusBeaconUtility", application);
        return application->run(*mainWindow);
    } catch (const Glib::Error &aError) {
        std::cerr << "Ошибка: " << aError.what() << std::endl;
        return 1;
    }
}