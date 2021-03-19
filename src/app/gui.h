#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "../sfb/sfb.h"

#define ID_MENU_OPEN 10
#define ID_MENU_CREATE 11
#define ID_MENU_SAVE 12 
#define ID_MENU_SAVEAS 13
#define ID_MENU_CLOSE 14
#define ID_MENU_EXIT 15

#define ID_BUTTON_SAVE 16

class MyApp : public wxApp 
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame 
{
public:
	MyFrame(const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	
private:
	// Events
	void MenuOpenEvent(wxCommandEvent& event);
	void MenuCreateEvent(wxCommandEvent& event);
	void MenuSaveEvent(wxCommandEvent& event);
	void MenuSaveAsEvent(wxCommandEvent& event);
	void MenuCloseEvent(wxCommandEvent& event);
	void MenuExitEvent(wxCommandEvent& event);

	void ButtonSaveEvent(wxCommandEvent& event);

	void OnDropFiles(wxDropFilesEvent& event);
	
	void OnWindowClose(wxCloseEvent& event);

	DECLARE_EVENT_TABLE()

	// Controls
	wxTextCtrl* TextCtrl[9];
	wxStaticText* StaticText[9];
	wxButton* SaveBut;
	wxMenu* Menu;
	wxMenuBar* Menubar;
	wxPanel* mainPanel;

	// Sizers
	wxBoxSizer* verticalSizer;
	wxBoxSizer* subboxsizer[9];
	wxBoxSizer* SaveButSizer;

	// Unimportant
	wxSize DefaultSize;
	SFB sfb;
	bool FileCreateMode = false;
	bool FileOpened = false;

	void CreateMyWidgets();
	void SetTextCtrls();
	void GetFromTextCtrls();
};