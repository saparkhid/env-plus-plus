#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <memory>
#include "MenuBar.h"
class MainWindow
{
public:
    MainWindow(HINSTANCE hInstance);
    ~MainWindow();
    bool Create();
  
    int Run();
    void Show(int nCmdShow = SW_SHOW);
    void Center();
    HWND GetHandle() const { return m_hwndMain; }
    HINSTANCE GetInstance() const { return m_hInstance; }

    // Menu management
    void SetMenuBar(MenuBar* menu);
private:
	HWND m_hwndMain;
    WCHAR m_szTitle[256];
    HINSTANCE m_hInstance;
    static MenuBar* menuBar;
    bool RegisterWindowClass();
    bool CreateWindowInstance();
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

