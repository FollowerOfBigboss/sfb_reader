#include <Windows.h>
#include <string.h>

#include "sfb.h"

#define DEBUG  1

#if DEBUG
#define DEBUG_ASSERT(message) { \
wchar_t wmsg[sizeof(message) + sizeof(__FILEW__) + sizeof(__LINE__) + sizeof(__FUNCTIONW__) + 41]; \
_snwprintf(wmsg, sizeof(message) + sizeof(__FILEW__) + sizeof(__LINE__) + sizeof(__FUNCTIONW__) + 41, L"FILE: %ls\nLINE: %d\nFUNC: %ls\nMESSAGE: %ls",__FILEW__, __LINE__, __FUNCTIONW__, message); \
MessageBoxW(NULL, wmsg, L"Error", MB_ICONERROR | MB_OK); \
__debugbreak(); \
}
#else
#define DEBUG_ASSERT(message) { \
wchar_t wmsg[ sizeof(message) + sizeof(__FILEW__) + sizeof(__LINE__) + sizeof(__FUNCTIONW__) + 41 ]; \
/*printf("%i\n", sizeof(message) + sizeof(__FILEW__) + sizeof(__LINE__) + sizeof(__FUNCTIONW__) + 41);*/\
_snwprintf(wmsg, sizeof(message) + sizeof(__FILEW__) + sizeof(__LINE__) + sizeof(__FUNCTIONW__) + 41, L"FILE: %ls\nLINE: %d\nFUNC: %ls\nMESSAGE: %ls",__FILEW__, __LINE__, __FUNCTIONW__, message); \
MessageBoxW(NULL, wmsg, L"Error", MB_ICONERROR | MB_OK); \
}
#endif

/* Error messages */
#define ERROR_CREATE_MESSAGE    L"Error occurred while creating file!"
#define ERROR_OPEN_MESSAGE      L"Error occurred while opening file!"
#define ERROR_WRITE_MESSAGE     L"Error occurred while writing file!"
#define ERROR_CLOSE_MESSAGE     L"Error occurred while closing file!"

static HWND window;
static HMENU Menubar;
static HMENU SMenu;
static wchar_t str[MAX_PATH];
static int created_new_file = 0;
static FILE* sfb_handle;
static SFB sfb;

static HWND EditControls[9];
const LPCWSTR static_texts[9] = { 
    L"version", 
    L"hybrid_flag", 
    L"disc_content_data_offset", 
    L"disc_content_data_lenght", 
    L"disc_title_name", 
    L"disc_title_data_offset", 
    L"disc_title_data_lenght", 
    L"disc_content", 
    L"disc_title" 
};


void CreateGuiApp();
void GuiAppRun();
LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void OpenFileDialog(HWND handle);
void SaveFileDialog(HWND handle, wchar_t* wstr);
HWND CreateEditControl(HWND hwnd, LPCWSTR s, int x, int y, int width, int height);
void CreateLabel(HWND hwnd, LPCWSTR s, int x, int y, int width, int height);
void CopyBoxInfosToStruct();
void SetWindowTitle(HWND handle, const wchar_t* titlestr);
void Release_avaible_handle(FILE** sfb_handle);

#define M_OPEN 0x0010
#define M_CREATE 0x0011
#define M_SAVE 0x0012
#define M_SAVEAS 0x0013
#define M_CLOSE 0x0014
#define M_EXIT 0x0015

void CreateGuiApp()
{
    WNDCLASSEXW wcexw;
    wcexw.cbSize = sizeof(WNDCLASSEXW);
    wcexw.style = (CS_HREDRAW | CS_VREDRAW);
    wcexw.lpfnWndProc = WindowProc;
    wcexw.cbClsExtra = 0;
    wcexw.cbWndExtra = 0;
    wcexw.hInstance = GetModuleHandleW(NULL);
    wcexw.hIcon = NULL;
    wcexw.hCursor = NULL;
    wcexw.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wcexw.lpszMenuName = NULL;
    wcexw.lpszClassName = L"SFB_window";
    wcexw.hIconSm = NULL;
    RegisterClassExW(&wcexw);

    window = CreateWindowExW(WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES | WS_EX_TOPMOST, L"SFB_window", L"SFB Editor", WS_OVERLAPPEDWINDOW, 0, 0, 500, 500, NULL, NULL, GetModuleHandleW(NULL), NULL);
    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
}

void GuiAppRun()
{
    MSG msg;
    while (GetMessageW(&msg, window, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        Menubar = CreateMenu();
        SMenu = CreateMenu();
        AppendMenuW(SMenu, MF_STRING, M_OPEN, L"&Open");
        AppendMenuW(SMenu, MF_STRING, M_CREATE, L"&Create");
        AppendMenuW(SMenu, MF_STRING, M_SAVE, L"&Save");
        AppendMenuW(SMenu, MF_STRING, M_SAVEAS, L"&Save As");

        AppendMenuW(SMenu, MF_STRING, M_CLOSE, L"&Close");
        AppendMenuW(SMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(SMenu, MF_STRING, M_EXIT, L"&Exit");
        
        AppendMenuW(Menubar, MF_POPUP, (UINT_PTR)SMenu, L"&File");
        
        SetMenu(hwnd, Menubar);

        CreateWindowExW(0L, L"BUTTON", L"Save", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 380, 408, 100, 30, hwnd, (HMENU)M_SAVE, GetModuleHandleW(NULL), NULL);

        int st = 50;
		int i;
        for (i = 0; i < 9; i++)
        {
            CreateLabel(hwnd, static_texts[i], 0, st, 200, 20);
            EditControls[i] = CreateEditControl(hwnd, L"", 200, st, 200, 25);
            st += 30;
        }
        break;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            case M_CREATE: 
            {
                if (sfb_handle != NULL)
                {
                    int msgbox = MessageBoxW(hwnd, L"All of changes that you didn't save will gone.\nDo you wanna create new sfb file?", L"Warning", MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2);
                    if (msgbox != IDYES)
                        break;
                    
                    _wcsset(str, L'\0');
                    Release_avaible_handle(&sfb_handle);
                }
                SetWindowTitle(hwnd, L"SFB Editor - New File");

                ZeroMemory(&sfb, sizeof(SFB));

                int i;
                for (i = 0; i < 9; i++)
                {
                    SetWindowTextA(EditControls[i], "");
                }

                SFB_set_defaults(&sfb);
                char t[20];
                snprintf(t, 20, "0x%.8X", REV(sfb.version));
                SetWindowTextA(EditControls[0], t);

                SetWindowTextA(EditControls[1], sfb.hybrid_flag);

                snprintf(t, 20, "0x%.8X", REV(sfb.disc_content_data_offset));
                SetWindowTextA(EditControls[2], t);

                snprintf(t, 20, "0x%.8X", REV(sfb.disc_content_data_lenght));
                SetWindowTextA(EditControls[3], t);

                SetWindowTextA(EditControls[4], sfb.disc_title_name);

                snprintf(t, 20, "0x%.8X", REV(sfb.disc_title_data_offset));
                SetWindowTextA(EditControls[5], t);

                snprintf(t, 20, "0x%.8X", REV(sfb.disc_title_data_lenght));
                SetWindowTextA(EditControls[6], t);

                SetWindowTextA(EditControls[7], sfb.disc_content);
                SetWindowTextA(EditControls[8], sfb.disc_title);

                created_new_file = 1;

                break; 
            }
            case M_OPEN: 
            {
                OpenFileDialog(hwnd);

                /* it cancelled likely */
                if (wcslen(str) == 0)
                {
                    break;
                }

                created_new_file = 0;
                Release_avaible_handle(&sfb_handle);

                sfb_handle = SFB_open_w(str);

                if (sfb_handle == SFB_FAIL)
                {
                    DEBUG_ASSERT(ERROR_OPEN_MESSAGE);
                    break;
                }

                if (SFB_read(&sfb, sfb_handle) == SFB_FAIL)
                {
                    DEBUG_ASSERT(L"Error occurred while reading!");
                    break;
                }

                if (strcmp(sfb.magic, "\x2E\x53\x46\x42") != 0)
                {
                    MessageBoxW(hwnd,L"Magic didn't match!", L"Error", MB_OK | MB_ICONWARNING);
                    Release_avaible_handle(&sfb_handle);
                    break;
                }

                {
                    wchar_t ws[MAX_PATH + 13];
                    _snwprintf(ws, MAX_PATH + 13, L"SFB Editor - %ls", str);
                    SetWindowTitle(hwnd, ws);
                }


                char t[20];
                snprintf(t, 20, "0x%.8X", REV(sfb.version));
                SetWindowTextA(EditControls[0], t);
                
                SetWindowTextA(EditControls[1], sfb.hybrid_flag);
                
                snprintf(t, 20, "0x%.8X", REV(sfb.disc_content_data_offset));
                SetWindowTextA(EditControls[2], t);

                snprintf(t, 20, "0x%.8X", REV(sfb.disc_content_data_lenght));
                SetWindowTextA(EditControls[3], t);
                
                SetWindowTextA(EditControls[4], sfb.disc_title_name);

                snprintf(t, 20, "0x%.8X", REV(sfb.disc_title_data_offset));
                SetWindowTextA(EditControls[5], t);

                snprintf(t, 20, "0x%.8X", REV(sfb.disc_title_data_lenght));
                SetWindowTextA(EditControls[6], t);

                SetWindowTextA(EditControls[7], sfb.disc_content);
                SetWindowTextA(EditControls[8], sfb.disc_title);

                break;
            }
            case M_SAVE:
            {
                FILE* file;
                CopyBoxInfosToStruct();
                /* New file created */
                if (created_new_file == 1)
                {
                    SaveFileDialog(hwnd, str);
                    if (wcslen(str) == 0)
                        break;

                    file = SFB_create_w(str, &sfb);

                    if (file == SFB_FAIL)
                    {
                        DEBUG_ASSERT(ERROR_CREATE_MESSAGE);
                        break;
                    }
                    
                    {
                        wchar_t tmp[MAX_PATH + 13];
                        _snwprintf(tmp, MAX_PATH + 13, L"SFB Editor - %ls", str);
                        SetWindowTitle(hwnd, tmp);
                    }
                    created_new_file = 0;
                }
                else if (wcslen(str) == 0)
                {
                    MessageBoxW(hwnd, L"There is no file opened or created!", L"Error", MB_ICONERROR | MB_OK);
                    break;
                }
                else
                {
                    file = SFB_open_w(str);
                    if (file == SFB_FAIL)
                    {
                        DEBUG_ASSERT(ERROR_OPEN_MESSAGE);
                        break;
                    }
                }
                
                if (SFB_write(&sfb, file) == SFB_FAIL)
                {
                    DEBUG_ASSERT(ERROR_WRITE_MESSAGE);
                    break;
                }
                
                if (SFB_close(file) == SFB_FAIL)
                {
                    DEBUG_ASSERT(ERROR_CLOSE_MESSAGE);
                }
                break;
            }

            case M_SAVEAS:
            {
                if (wcslen(str) > 0)
                {
                    wchar_t SaveAs[MAX_PATH] = L"";
                    CopyBoxInfosToStruct();
                    SaveFileDialog(hwnd, SaveAs);

                    /* Cancelled */
                    if (wcslen(SaveAs) == 0)
                        break;

                    FILE* file = SFB_create_w(SaveAs, &sfb);

                    if (file == SFB_FAIL)
                    {
                        DEBUG_ASSERT(ERROR_OPEN_MESSAGE);
                        break;
                    }

                    if (SFB_write(&sfb, file) == SFB_FAIL)
                    {
                        DEBUG_ASSERT(ERROR_WRITE_MESSAGE);
                        break;
                    }

                    if (SFB_close(file) != SFB_SUCCESSFUL)
                    {
                        DEBUG_ASSERT(ERROR_CLOSE_MESSAGE);
                    }
                }
                break;
            }

            case M_CLOSE:
            {
                created_new_file = 0;
                Release_avaible_handle(&sfb_handle);
                
                _wcsset(str, L'\0');
                int i;
                for (i = 0; i < 9; i++)
                {
                    SetWindowTextA(EditControls[i], "");
                }

                SetWindowTextW(hwnd, L"SFB Editor");
                break;
            }

            case M_EXIT:
            {
                /* Just post WM_CLOSE message */
                PostMessageW(hwnd, WM_CLOSE, 0, 0);
                break;
            }
        }
        break;
    }

    case WM_DROPFILES:
    {
        HDROP drop = (HDROP)wParam;
        DragQueryFileW(drop, 0, str, MAX_PATH);
        
        Release_avaible_handle(&sfb_handle);
        
        sfb_handle = SFB_open_w(str);
        if (sfb_handle == SFB_FAIL)
        {
            DEBUG_ASSERT(ERROR_OPEN_MESSAGE);
            break;
        }

        SFB_read(&sfb, sfb_handle);

        if (strcmp(sfb.magic, "\x2E\x53\x46\x42") != 0)
        {
            MessageBoxA(hwnd, "Magic didn't match!", "Error", MB_ICONERROR | MB_OK);
            break;
        }

        {
            wchar_t ws[MAX_PATH + 13];
            _snwprintf(ws, MAX_PATH + 13, L"SFB Editor - %ls", str);
            SetWindowTitle(hwnd, ws);
        }


        char t[20];
        snprintf(t, 20, "0x%.8X", REV(sfb.version));
        SetWindowTextA(EditControls[0], t);

        SetWindowTextA(EditControls[1], sfb.hybrid_flag);

        snprintf(t, 20, "0x%.8X", REV(sfb.disc_content_data_offset));
        SetWindowTextA(EditControls[2], t);

        snprintf(t, 20, "0x%.8X", REV(sfb.disc_content_data_lenght));
        SetWindowTextA(EditControls[3], t);

        SetWindowTextA(EditControls[4], sfb.disc_title_name);

        snprintf(t, 20, "0x%.8X", REV(sfb.disc_title_data_offset));
        SetWindowTextA(EditControls[5], t);

        snprintf(t, 20, "0x%.8X", REV(sfb.disc_title_data_lenght));
        SetWindowTextA(EditControls[6], t);

        SetWindowTextA(EditControls[7], sfb.disc_content);
        SetWindowTextA(EditControls[8], sfb.disc_title);

        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_CLOSE:
    {
        /* Close handle before close window */
        Release_avaible_handle(&sfb_handle);
        DestroyWindow(hwnd);
        break;
    }
    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void OpenFileDialog(HWND handle)
{
    OPENFILENAMEW opfw;
    ZeroMemory(&opfw, sizeof(OPENFILENAMEW));
    opfw.lStructSize = sizeof(OPENFILENAMEW);
    opfw.hwndOwner = handle;
    opfw.hInstance = GetModuleHandleW(NULL);
    opfw.lpstrFilter = L"PS3_DISC.SFB\0*.sfb\0All files\0*.*\0";
    opfw.lpstrFile = str;
    opfw.nMaxFile = MAX_PATH;
    GetOpenFileNameW(&opfw);
}

void SaveFileDialog(HWND handle, wchar_t* wstr)
{
    OPENFILENAMEW opfw;
    ZeroMemory(&opfw, sizeof(OPENFILENAMEW));
    opfw.lStructSize = sizeof(OPENFILENAMEW);
    opfw.hwndOwner = handle;
    opfw.hInstance = GetModuleHandleW(NULL);
    opfw.lpstrFilter = L"PS3_DISC.SFB\0*.sfb\0All files\0*.*\0";
    opfw.lpstrFile = wstr;
    opfw.nMaxFile = MAX_PATH;
    opfw.Flags = OFN_OVERWRITEPROMPT;
    GetSaveFileNameW(&opfw);
}

HWND CreateEditControl(HWND hwnd,LPCWSTR s, int x, int y, int width, int height)
{
    return CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", s, ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD, x, y, width, height, hwnd, NULL, GetModuleHandleW(NULL), NULL);
}

void CreateLabel(HWND hwnd, LPCWSTR s, int x, int y, int width, int height)
{
    CreateWindowExW(0L, L"STATIC", s, WS_CHILD | WS_VISIBLE, x, y, width, height, hwnd, NULL, GetModuleHandleW(NULL), NULL);
}

void CopyBoxInfosToStruct()
{
    char Text[20];
    int TextLenghts[9];

    const int expectedLenghts[9] =
    {
        10, /* version calculated with 0x*/
        11, /* hybrid flag */
        10, /* disc content data offset calculated with 0x */
        10, /* disc content data lenght calculated with 0x */
        8, /* title id */
        10, /* disc title data offset calculated with 0x */
        10, /* disc title data lenght calculated with 0x */
        32, /* disc content */
        16 /* disc title */
    };

    /* 
    More work needs
    - Check lengths of strings that boxes keeps
    - Check beginnings of strings
    */
    int i;
    for (i = 0; i < 9; i++)
    {
        TextLenghts[i] = GetWindowTextLengthA(EditControls[i]);
        
        /* temporary */
        if (TextLenghts[i] > expectedLenghts[i])
        {
            MessageBoxW(window, L"Some boxes keeping more string than it should be", L"Warning", MB_ICONWARNING | MB_OK);
            return;
        }

    }   
    


    GetWindowTextA(EditControls[0], Text, 20);
    uint32_t test = (uint32_t)strtol(Text, NULL, 0);
    sfb.version = REV(test);

    GetWindowTextA(EditControls[1], Text, 20);
    strcpy(sfb.hybrid_flag, Text);

    GetWindowTextA(EditControls[2], Text, 20);
    test = (uint32_t)strtol(Text, NULL, 0);
    sfb.disc_content_data_offset = REV(test);

    GetWindowTextA(EditControls[3], Text, 20);
    test = (uint32_t)strtol(Text, NULL, 0);
    sfb.disc_content_data_lenght = REV(test);

    GetWindowTextA(EditControls[4], Text, 20);
    strcpy(sfb.disc_title_name, Text);

    GetWindowTextA(EditControls[5], Text, 20);
    test = (uint32_t)strtol(Text, NULL, 0);
    sfb.disc_title_data_offset = REV(test);

    GetWindowTextA(EditControls[6], Text, 20);
    test = (uint32_t)strtol(Text, NULL, 0);
    sfb.disc_title_data_lenght = REV(test);

    GetWindowTextA(EditControls[7], Text, 20);
    strcpy(sfb.disc_content, Text);

    GetWindowTextA(EditControls[8], Text, 20);
    strcpy(sfb.disc_title, Text);
}

void SetWindowTitle(HWND handle, const wchar_t* titlestr)
{
    if (SetWindowTextW(handle, titlestr) == FALSE)
    {
        DEBUG_ASSERT(L"Error occurred while setting window title!");
    }
}

void Release_avaible_handle(FILE** sfb_handle)
{
    if (*sfb_handle != NULL)
    {
        if (SFB_close(*sfb_handle) == SFB_FAIL)
        {
            DEBUG_ASSERT(ERROR_CLOSE_MESSAGE);
        }
        *sfb_handle = NULL;
    }
}