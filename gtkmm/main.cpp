//
// Main routine of the application.
//
// Copyright (c) 2023 Serge Vakulenko <serge.vakulenko@gmail.com>
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//
#include <gtkmm/application.h>

#include "main_window.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.sergev.calculator");

    // Shows the window and returns when it is closed.
    return app->make_window_and_run<Main_Window>(argc, argv);
}
