#include "main_window.h"

//
// Define custom application class.
//
class Application : public wxApp {
public:
    // Initialize application on startup.
    virtual bool OnInit() wxOVERRIDE;
};

//
// Create a new application object.
//
wxIMPLEMENT_APP(Application);

//
// The program execution "starts" here.
//
bool Application::OnInit()
{
    // Initialize the base class.
    if (!wxApp::OnInit())
        return false;

    // Create the main application window.
    auto *frame = new MainWindow("TI-2500 Datamath");

    // Display the main window.
    frame->Show(true);

    return true;
}
