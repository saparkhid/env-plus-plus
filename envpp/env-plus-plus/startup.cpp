#include <windows.h>
#include <commctrl.h>


// Menu command IDs
#define ID_NEW  1001
#define ID_EXIT 1002

// Global variables
HWND hwndMain;
HMENU hMenu;

#pragma comment(linker, \
"\"/manifestdependency:type='win32' "\
"name='Microsoft.Windows.Common-Controls' "\
"version='6.0.0.0' "\
"processorArchitecture='*' "\
"publicKeyToken='6595b64144ccf1df' "\
"language='*'\"")


#pragma comment(lib, "comctl32.lib")
// Callback for messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_NEW:
            MessageBox(hwnd, L"New File Action!", L"Info", MB_OK);
            break;
        case ID_EXIT:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) 
{
    const wchar_t CLASS_NAME[] = L"EnvppWin32";

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);

    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    // Create main window
    hwndMain = CreateWindowEx(
        0,
        CLASS_NAME,
        L"ENV++",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1000, 700,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwndMain) return 0;

    // Create menu
    hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, ID_NEW, L"New");
    AppendMenu(hFileMenu, MF_STRING, ID_EXIT, L"Exit");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    SetMenu(hwndMain, hMenu);


    RECT rc;
    GetWindowRect(hwndMain, &rc);
    int windowWidth = rc.right - rc.left;
    int windowHeight = rc.bottom - rc.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int xPos = (screenWidth - windowWidth) / 2;
    int yPos = (screenHeight - windowHeight) / 2;

    SetWindowPos(hwndMain, nullptr, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

 
    ShowWindow(hwndMain, iCmdShow);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
