#include "main_window.h"

// IDs for the controls and the menu commands
enum {
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT
};

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
// clang-format off
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(Minimal_Quit,  MainWindow::OnQuit)
    EVT_MENU(Minimal_About, MainWindow::OnAbout)
wxEND_EVENT_TABLE()
// clang-format on

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "icon.xpm"
#endif

    // frame constructor
    MainWindow::MainWindow(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUBAR
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#else  // !wxUSE_MENUBAR
    // If menus are not available add a button to access the about box
    wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *aboutBtn = new wxButton(this, wxID_ANY, "About...");
    aboutBtn->Bind(wxEVT_BUTTON, &MainWindow::OnAbout, this);
    sizer->Add(aboutBtn, wxSizerFlags().Center());
    SetSizer(sizer);
#endif // wxUSE_MENUBAR/!wxUSE_MENUBAR

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");
#endif // wxUSE_STATUSBAR
}

// event handlers

void MainWindow::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxMessageBox(wxString::Format("Welcome to %s!\n"
                                  "\n"
                                  "This is the minimal wxWidgets sample\n"
                                  "running under %s.",
                                  wxVERSION_STRING, wxGetOsDescription()),
                 "About wxWidgets minimal sample", wxOK | wxICON_INFORMATION, this);
}
