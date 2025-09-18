#include "MenuBar.h"

MenuBar::MenuBar() {
    hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING, ID_NEW, L"New");
    AppendMenu(hFileMenu, MF_STRING, ID_EXIT, L"Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
}

MenuBar::~MenuBar() {
    DestroyMenu(hMenu);
}

void MenuBar::HandleCommand(WORD id) {
    switch (id) {
    case ID_NEW:
        if (onNew) onNew();
        break;
    case ID_EXIT:
        if (onExit) onExit();
        break;
    }
}
