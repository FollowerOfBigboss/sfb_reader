#include "gui.h"
#include <string>

BEGIN_EVENT_TABLE(MyFrame, wxFrame)

    EVT_MENU(ID_MENU_OPEN, MyFrame::MenuOpenEvent)
    EVT_MENU(ID_MENU_CREATE, MyFrame::MenuCreateEvent)
    EVT_MENU(ID_MENU_SAVE, MyFrame::MenuSaveEvent)
    EVT_MENU(ID_MENU_SAVEAS, MyFrame::MenuSaveAsEvent)
    EVT_MENU(ID_MENU_CLOSE, MyFrame::MenuCloseEvent)
    EVT_MENU(ID_MENU_EXIT, MyFrame::MenuExitEvent)
    EVT_BUTTON(ID_BUTTON_SAVE, MyFrame::ButtonSaveEvent)

    EVT_DROP_FILES(MyFrame::OnDropFiles)
    EVT_CLOSE(MyFrame::OnWindowClose)

END_EVENT_TABLE()


const char* StaticTextLabels[] = {
    "Version",
    "Hybrid Flag",
    "Disc Content Data Offset",
    "Disc Content Data Lenght",
    "Disc Title Name",
    "Disc Title Data Offset",
    "Disc Title Data Lenght",
    "Disc Content",
    "Disc Title"
};

bool MyApp::OnInit()
{
	MyFrame* MainWin = new MyFrame(wxT("sfb reader"), wxPoint(0,0), wxSize(400, 400));
	MainWin->Show(true);
	return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, -1, title, pos, size)
{
    DefaultSize = size;
    this->CreateMyWidgets();
}

void MyFrame::MenuOpenEvent(wxCommandEvent& event)
{
    FileCreateMode = 0;

    if (FileOpened == true)
    {
        sfb.close();
        FileOpened = false;
    }

    wxFileDialog* dialog = new wxFileDialog(this, wxT("Sfb files"), wxT(""), wxT(""), wxT("SFB Files (*.sfb)|*.sfb|All Files (*.*)|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dialog->ShowModal() == wxID_OK)
    {
    #ifdef _WIN32
            sfb.open(dialog->GetPath().wc_str());
    #else
        sfb.open(dialog->GetPath().mb_str());
    #endif
        
        sfb.read();
        SetTextCtrls();
        this->SetLabel(wxString::Format(wxT("SFB Editor - %s"), dialog->GetPath()));
        FileOpened = true;
    }
}

void MyFrame::MenuCreateEvent(wxCommandEvent& event)
{
    if (FileOpened == true)
    {
        sfb.close();
    }
    FileCreateMode = true;
    this->SetLabel( wxString::Format(wxT("SFB Editor - New Project")) );
    sfb.set_defaults();
    SetTextCtrls();
}

void MyFrame::MenuSaveEvent(wxCommandEvent& event)
{
    if (FileCreateMode == true)
    {
        wxFileDialog* dialog = new wxFileDialog(this, wxT("Sfb files"), wxT(""), wxT(""), wxT("SFB Files (*.sfb)|*.sfb|All Files (*.*)|*.*"), wxFD_SAVE);
        if (dialog->ShowModal() == wxID_OK)
        {
        #ifdef _WIN32
            sfb.create(dialog->GetPath().wc_str());
        #else
            sfb.create(dialog->GetPath().mb_str())
        #endif
            GetFromTextCtrls();
            sfb.write();
            FileCreateMode = false;
            FileOpened = true;
            this->SetLabel(wxString::Format(wxT("SFB Editor - %s"), dialog->GetPath()));
        }
        return;
    }

    if (FileOpened == true)
    {
        GetFromTextCtrls();
        sfb.write();
        return;
    }
}

void MyFrame::MenuSaveAsEvent(wxCommandEvent& event)
{
    if (FileOpened == 1 || FileCreateMode == 1)
    {
        wxFileDialog* dialog = new wxFileDialog(this, wxT("Sfb files"), wxT(""), wxT(""), wxT("SFB Files (*.sfb)|*.sfb|All Files (*.*)|*.*"), wxFD_SAVE);
        if (dialog->ShowModal() == wxID_OK)
        {
        #ifdef _WIN32
            void* ptr = sfb.create_as(dialog->GetPath().wc_str());
        #else
            void* ptr = sfb.create_as(dialog->GetPath().mb_str());
        #endif
            sfb.write_as(ptr);
            sfb.close_as(ptr);
        }
    }
}

void MyFrame::MenuCloseEvent(wxCommandEvent& event)
{
    if (FileOpened == true)
    {
        sfb.close();
        this->SetLabel(wxT("SFB Editor"));
        FileOpened = false;
    }

    if (FileCreateMode == true)
    {
        FileCreateMode = false;
        this->SetLabel(wxT("SFB Editor"));
    }

    for (int i = 0; i < 9; i++)
    {
        TextCtrl[i]->SetLabel(wxT(""));
    }
}

void MyFrame::MenuExitEvent(wxCommandEvent& event)
{
    this->Close();
}

void MyFrame::ButtonSaveEvent(wxCommandEvent& event)
{
    if (FileCreateMode == true)
    {
        wxFileDialog* dialog = new wxFileDialog(this, wxT("Sfb files"), wxT(""), wxT(""), wxT("SFB Files (*.sfb)|*.sfb|All Files (*.*)|*.*"), wxFD_SAVE);
        if (dialog->ShowModal() == wxID_OK)
        {
        #ifdef _WIN32
            sfb.create(dialog->GetPath().wc_str());
        #else
            sfb.create(dialog->GetPath().mb_str());
        #endif
        
        GetFromTextCtrls();
        sfb.write();
        FileCreateMode = false;
        FileOpened = true;
        this->SetLabel(wxString::Format(wxT("SFB Editor - %s"), dialog->GetPath()));
        }
    }
    else
    {

        GetFromTextCtrls();
        sfb.write();
    }
}

void MyFrame::OnDropFiles(wxDropFilesEvent& event)
{
    if (FileCreateMode == 1)
    {
        FileCreateMode = 0;
    }

    if (FileOpened == true)
    {
        sfb.close();
    }

    #ifdef _WIN32
        sfb.open(event.GetFiles()[0].wc_str());
    #else
        sfb.open(event.GetFiles()[0].mb_str());
    #endif

    sfb.read();
    SetTextCtrls();
    this->SetLabel( wxString::Format(wxT("SFB Editor - %s"), event.GetFiles()[0].ToStdString()) );
    FileOpened = true;
}

void MyFrame::OnWindowClose(wxCloseEvent& event)
{
    if (FileOpened == true)
    {
        sfb.close();
    }
    this->Destroy();
}

void MyFrame::CreateMyWidgets()
{
    
    this->DragAcceptFiles(true);

    Menu = new wxMenu;
    Menu->Append(ID_MENU_OPEN, wxT("Open"));
    Menu->Append(ID_MENU_CREATE, wxT("Create"));
    Menu->Append(ID_MENU_SAVE, wxT("Save"));
    Menu->Append(ID_MENU_SAVEAS, wxT("Save As"));
    Menu->Append(ID_MENU_CLOSE, wxT("Close"));
    Menu->AppendSeparator();
    Menu->Append(ID_MENU_EXIT, wxT("Exit"));

    Menubar = new wxMenuBar();
    Menubar->Append(Menu, wxT("File"));
    SetMenuBar(Menubar);

    mainPanel = new wxPanel(this, wxID_ANY);
    mainPanel->SetBackgroundColour(wxColour(192, 192, 192));
   
    verticalSizer = new wxBoxSizer(wxVERTICAL);
   
    for (int i = 0; i < 9; i++)
    {
        subboxsizer[i]  = new wxBoxSizer(wxHORIZONTAL);
        StaticText[i]   = new wxStaticText(mainPanel, wxID_ANY, StaticTextLabels[i], wxDefaultPosition, wxSize(150, 22));
        // StaticText[i]->SetBackgroundColour(wxColour(240, 240, 240));
        TextCtrl[i]     = new wxTextCtrl(mainPanel, wxID_ANY);
        
        subboxsizer[i]->Add(StaticText[i], 0, wxALL, 5);
        subboxsizer[i]->Add(TextCtrl[i], 1, wxALL, 5);
        
        verticalSizer->Add(subboxsizer[i], 1, wxEXPAND, 10);
    }

    SaveBut      = new wxButton(mainPanel, ID_BUTTON_SAVE, wxT("Save"));
    SaveButSizer = new wxBoxSizer(wxHORIZONTAL);
    
    SaveButSizer->Add(SaveBut);
    verticalSizer->Add(SaveButSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

    mainPanel->SetSizer(verticalSizer);
    mainPanel->Fit();
    
    this->Fit();
    wxSize Wsize = this->GetSize();
    this->SetMinSize(Wsize);
    this->SetSize(DefaultSize);
    
    this->SetIcon(wxNullIcon);
}

void MyFrame::SetTextCtrls()
{
    TextCtrl[0]->SetLabelText(wxString::Format(wxT("0x%.8X"), REV(sfb.version)));
    TextCtrl[1]->SetLabelText(sfb.hybrid_flag);
    TextCtrl[2]->SetLabelText(wxString::Format(wxT("0x%.8X"), REV(sfb.disc_content_data_offset)));
    TextCtrl[3]->SetLabelText(wxString::Format(wxT("0x%.8X"), REV(sfb.disc_content_data_lenght)));
    TextCtrl[4]->SetLabelText(sfb.disc_title_name);
    TextCtrl[5]->SetLabelText(wxString::Format(wxT("0x%.8X"), REV(sfb.disc_title_data_offset)));
    TextCtrl[6]->SetLabelText(wxString::Format(wxT("0x%.8X"), REV(sfb.disc_title_data_lenght)));
    TextCtrl[7]->SetLabelText(sfb.disc_content);
    TextCtrl[8]->SetLabelText(sfb.disc_title);
}

void MyFrame::GetFromTextCtrls()
{
    wxString tmp;
    tmp =  TextCtrl[0]->GetValue();
    sfb.version = REV(wxStrtol(tmp, NULL, 0));


    tmp = TextCtrl[1]->GetValue();
    strcpy_s(sfb.hybrid_flag, sizeof(sfb.hybrid_flag), tmp.mbc_str());

    tmp = TextCtrl[2]->GetValue();
    sfb.disc_content_data_offset = REV(wxStrtol(tmp, NULL, 0));

    tmp = TextCtrl[3]->GetValue();
    sfb.disc_content_data_lenght = REV(wxStrtol(tmp, NULL, 0));

    tmp = TextCtrl[4]->GetValue();
    strcpy(sfb.disc_title_name, tmp.mbc_str());

    tmp = TextCtrl[5]->GetValue();
    sfb.disc_title_data_offset = REV(wxStrtol(tmp, NULL, 0));

    tmp = TextCtrl[6]->GetValue();
    sfb.disc_title_data_lenght = REV(wxStrtol(tmp, NULL, 0));

    tmp = TextCtrl[7]->GetValue();
    strcpy(sfb.disc_content, tmp.mbc_str());

    tmp = TextCtrl[8]->GetValue();
    strcpy(sfb.disc_title, tmp.mbc_str());

}
