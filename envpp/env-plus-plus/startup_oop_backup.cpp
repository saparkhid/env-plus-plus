/* I can replace this file with startup file*/

//#include <windows.h>
//#include <commctrl.h>
//#include <vector>
//#include <string>
//#include "MainWindow.h"
//#include "resource.h"
//#include "MenuBar.h"
//
//
//#pragma comment(linker, \
//"\"/manifestdependency:type='win32' "\
//"name='Microsoft.Windows.Common-Controls' "\
//"version='6.0.0.0' "\
//"processorArchitecture='*' "\
//"publicKeyToken='6595b64144ccf1df' "\
//"language='*'\"")
//
//#pragma comment(lib, "comctl32.lib")
////
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
//    // Create and initialize the main window
//    MainWindow mainWindow(hInstance);
//    if (!mainWindow.Create()) {
//        return 0;
//    }
//    // Center the window
//    mainWindow.Center();
//    // Show the window
//    mainWindow.Show(iCmdShow);
//
//
//    MenuBar menu;
//    menu.SetOnNew([]() {
//        MessageBox(NULL, L"New File Action!", L"Info", MB_OK);
//        });
//
//    menu.SetOnExit([&]() {
//        PostQuitMessage(0);
//        });
//
//    mainWindow.SetMenuBar(&menu);
//
//
//
//    // Run the message loop
//    return mainWindow.Run();
//}
//
