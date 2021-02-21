#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "Gdi32.lib")

#include <iostream>
#include <Windows.h>
#include <string.h>

#include "sfb.h"

HWND window;
HMENU Menubar;
HMENU SMenu;
wchar_t str[MAX_PATH];
int created_new_file = 0;
FILE* sfb_handle;
SFB sfb;

HWND EditControls[9];
LPCWSTR static_texts[9] = { L"version", L"hybrid_flag", L"disc_content_data_offset", L"disc_content_data_lenght", L"disc_title_name", L"disc_title_data_offset", L"disc_title_data_lenght", L"disc_content", L"disc_title" };


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
    wcexw.cbClsExtra = NULL;
    wcexw.cbWndExtra = NULL;
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
    while (GetMessageW(&msg, window, NULL, NULL) > 0)
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
        for (int i = 0; i < 9; i++)
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

                created_new_file = 0;
                // it cancelled likely
                if (wcslen(str) == 0)
                {
                    // __debugbreak();
                    break;
                }

                Release_avaible_handle(&sfb_handle);

                sfb_handle = SFB_open(str);

                if (sfb_handle == SFB_OPEN_FAIL)
                {
                    MessageBoxW(hwnd, L"Error occurred while opening file!", L"Error", MB_OK | MB_ICONERROR);
                    break;
                }

                SFB_read(&sfb, sfb_handle);

                if (strcmp(sfb.magic, "\x2E\x53\x46\x42") != 0)
                {
                    MessageBoxW(hwnd, L"Magic didn't match!", L"Error", MB_ICONERROR | MB_OK);
                    Release_avaible_handle(&sfb_handle);
                    break;
                }

                {
                    wchar_t ws[MAX_PATH + 13] = L"SFB Editor - ";
                    wcscat(ws, str);
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
                    file = SFB_create(str, &sfb);
                    
                    {
                        wchar_t tmp[MAX_PATH + 13] = L"SFB Editor - ";
                        wcscat(tmp, str);
                        SetWindowTitle(hwnd, tmp);
                    }
                    
                }
                else if (wcslen(str) == 0)
                {
                    MessageBoxW(hwnd, L"There is no file opened or created!", L"Error", MB_ICONERROR | MB_OK);
                    break;
                }
                else
                {
                    file = SFB_open(str);
                }
                
                SFB_write(&sfb, file);
                SFB_close(file);
                break;
            }

            case M_SAVEAS:
            {
                wchar_t SaveAs[MAX_PATH] = L"";
                CopyBoxInfosToStruct();
                SaveFileDialog(hwnd, SaveAs);
                
                /* Cancelled */
                if (wcslen(SaveAs) == 0)
                    break;

                FILE* file = SFB_create(SaveAs, &sfb);
                SFB_write(&sfb, file);
                SFB_close(file);
                
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
                PostMessageW(hwnd, WM_CLOSE, NULL, NULL);
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
        
        sfb_handle = SFB_open(str);
        SFB_read(&sfb, sfb_handle);

        if (strcmp(sfb.magic, "\x2E\x53\x46\x42") != 0)
        {
            MessageBoxA(hwnd, "Magic didn't match!", "Error", MB_ICONERROR | MB_OK);
            break;
        }

        {
            wchar_t ws[MAX_PATH + 13] = L"SFB Editor - ";
            wcscat(ws, str);
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
    opfw.lpstrFilter = L"PS3_DISC.SFB\0\*.sfb\0All files\0\*.*\0";
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
    opfw.lpstrFilter = L"PS3_DISC.SFB\0\*.sfb\0All files\0\*.*\0";
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
    int len = GetWindowTextLengthA(EditControls[0]);

    GetWindowTextA(EditControls[0], Text, 15);
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
    SetWindowTextW(handle, titlestr);
}

void Release_avaible_handle(FILE** sfb_handle)
{
    if (*sfb_handle != NULL)
    {
        SFB_close(*sfb_handle);
        *sfb_handle = NULL;
    }
}