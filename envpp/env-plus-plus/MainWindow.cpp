//#include "MainWindow.h"
//#include <commctrl.h>
//#include "resource.h"
//
//
//MenuBar* MainWindow::menuBar = nullptr;
//
//MainWindow::MainWindow(HINSTANCE hInstance)
//    : m_hInstance(hInstance), m_hwndMain(nullptr) {
//    m_szTitle[0] = L'\0';
//}
//
//MainWindow::~MainWindow() {
//    // Cleanup if needed
//}
//
//bool MainWindow::Create() {
// 
//    INITCOMMONCONTROLSEX icc;
//    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
//    icc.dwICC = ICC_WIN95_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES;
//    InitCommonControlsEx(&icc);
//   
//    if (!RegisterWindowClass()) {
//        MessageBox(nullptr, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
//        return false;
//    }
//
//    if (!CreateWindowInstance()) {
//        MessageBox(nullptr, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
//        return false;
//    }
//
//    return true;
//}
//
//bool MainWindow::RegisterWindowClass() {
//
//    WNDCLASSEX wc = {};
//    wc.cbSize = sizeof(WNDCLASSEX);
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpfnWndProc = WndProc; 
//    wc.hInstance = m_hInstance;
//    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
//    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW +1);
//    wc.lpszClassName = L"EnvppMainWindowClass";
//
//    return RegisterClassEx(&wc) != 0;
//}
//
//bool MainWindow::CreateWindowInstance() {
//    // Load window title from string resource
//    LoadStringW(m_hInstance, IDS_TITLE, m_szTitle, sizeof(m_szTitle) / sizeof(WCHAR));
//
//    m_hwndMain = CreateWindowEx(
//        0,
//        L"EnvppMainWindowClass",
//        m_szTitle,
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
//        nullptr, nullptr, m_hInstance, nullptr
//    );
//
//    return m_hwndMain != nullptr;
//}
//
//void MainWindow::Show(int nCmdShow) {
//    if (m_hwndMain) {
//        ShowWindow(m_hwndMain, nCmdShow);
//        UpdateWindow(m_hwndMain);
//    }
//}
//
//void MainWindow::Center() {
//    if (!m_hwndMain) return;
//
//    RECT rc;
//    GetWindowRect(m_hwndMain, &rc);
//    int windowWidth = rc.right - rc.left;
//    int windowHeight = rc.bottom - rc.top;
//
//    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
//    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
//
//    int xPos = (screenWidth - windowWidth) / 2;
//    int yPos = (screenHeight - windowHeight) / 2;
//
//    SetWindowPos(m_hwndMain, nullptr, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
//}
//
//int MainWindow::Run() {
//    if (!m_hwndMain) {
//        return 0;
//    }
//    MSG msg;
//    while (GetMessage(&msg, nullptr, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    return msg.wParam;
//}
//
//LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
// 
//    switch (msg) {
//    case WM_COMMAND:
//        if (menuBar) {
//            menuBar->HandleCommand(LOWORD(wParam));
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//    case WM_CREATE:
//        // not strictly needed, but you could init stuff here
//        break;
//    }
//    return DefWindowProc(hwnd, msg, wParam, lParam);
//}
//
//void MainWindow::SetMenuBar(MenuBar* menu) {
//    menuBar = menu;
//    if (menuBar && m_hwndMain) {
//        SetMenu(m_hwndMain, menuBar->GetHandle());
//        
//    }
//}
