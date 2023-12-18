#include "main_window.h"

//
// Event table connects the wxWidgets events with functions which process them.
//
// clang-format off
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT, MainWindow::OnQuit)
    EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
wxEND_EVENT_TABLE()
// clang-format on

//
// The application icon.
//
#ifndef wxHAS_IMAGES_IN_RESOURCES
#   include "icon.xpm"
#endif

//
// Frame constructor.
//
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
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

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

//
// Event handlers.
//
void MainWindow::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    // True is to force the frame to close.
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
