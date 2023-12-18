#include <wx/wx.h>

// Define a new frame type: this is going to be our main frame
class MainWindow : public wxFrame {
public:
    // ctor(s)
    MainWindow(const wxString &title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

private:
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};
