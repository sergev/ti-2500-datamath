#include <wx/wx.h>

//
// Main window of our application.
//
class MainWindow : public wxFrame {
public:
    MainWindow(const wxString &title);

    // Event handlers.
    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

private:
    // Mandatory event table.
    wxDECLARE_EVENT_TABLE();
};
