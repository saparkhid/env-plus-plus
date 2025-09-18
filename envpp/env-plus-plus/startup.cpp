#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <string>



#pragma comment(lib, "comctl32.lib")

// Global variables
HWND g_hwndMain;
HWND g_hwndToolbar;
HWND g_hwndTreeView;
HWND g_hwndContainer;
HWND g_hwndTextBox;
HIMAGELIST g_hImageList;
HFONT g_hTextFont;

// Tree view node structure
struct TreeNode {
    std::wstring text;
    HTREEITEM hItem;
    TreeNode* parent;
    std::vector<TreeNode*> children;

    TreeNode(const std::wstring& txt, HTREEITEM item, TreeNode* p = nullptr)
        : text(txt), hItem(item), parent(p) {
    }

    ~TreeNode() {
        for (auto child : children) {
            delete child;
        }
    }
};

TreeNode* g_rootNode = nullptr;

// Function declarations
void CreateMenuBar(HWND hwnd);
void CreateToolbar(HWND hwnd);
void CreateTreeView(HWND hwnd);
void CreateTextBox(HWND hwnd);
void AddTreeNode(const std::wstring& text, HTREEITEM hParent = TVI_ROOT);
void RemoveSelectedTreeNode();
void InitializeCommonControls();
void UpdateLayout(int width, int height);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TextBoxProc(HWND, UINT, WPARAM, LPARAM);

// Text box subclassing
WNDPROC g_OriginalTextBoxProc = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    InitializeCommonControls();

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"MainWindowClass";

    if (!RegisterClassEx(&wc)) {
        MessageBox(nullptr, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    g_hwndMain = CreateWindowEx(
        0,
        L"MainWindowClass",
        L"Win32 Application with TreeView and Text Editor",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
        nullptr, nullptr, hInstance, nullptr
    );

    if (g_hwndMain == nullptr) {
        MessageBox(nullptr, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(g_hwndMain, iCmdShow);
    UpdateWindow(g_hwndMain);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void InitializeCommonControls() {
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);
}

void CreateMenuBar(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    HMENU hEditMenu = CreatePopupMenu();
    HMENU hTreeMenu = CreatePopupMenu();
    HMENU hViewMenu = CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING, 1001, L"&New");
    AppendMenu(hFileMenu, MF_STRING, 1002, L"&Open");
    AppendMenu(hFileMenu, MF_STRING, 1003, L"&Save");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hFileMenu, MF_STRING, 1004, L"E&xit");

    AppendMenu(hEditMenu, MF_STRING, 2001, L"&Undo");
    AppendMenu(hEditMenu, MF_STRING, 2002, L"&Cut");
    AppendMenu(hEditMenu, MF_STRING, 2003, L"&Copy");
    AppendMenu(hEditMenu, MF_STRING, 2004, L"&Paste");
    AppendMenu(hEditMenu, MF_STRING, 2005, L"Select &All");
    AppendMenu(hEditMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hEditMenu, MF_STRING, 2006, L"&Find");
    AppendMenu(hEditMenu, MF_STRING, 2007, L"&Replace");

    AppendMenu(hTreeMenu, MF_STRING, 3001, L"&Add Node");
    AppendMenu(hTreeMenu, MF_STRING, 3002, L"&Remove Node");
    AppendMenu(hTreeMenu, MF_STRING, 3003, L"&Clear All");
    AppendMenu(hTreeMenu, MF_STRING, 3004, L"&Expand All");
    AppendMenu(hTreeMenu, MF_STRING, 3005, L"&Collapse All");

    AppendMenu(hViewMenu, MF_STRING, 4001, L"&Word Wrap");
    AppendMenu(hViewMenu, MF_STRING, 4002, L"&Font...");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"&Edit");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hTreeMenu, L"&Tree");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"&View");

    SetMenu(hwnd, hMenu);
}

void CreateToolbar(HWND hwnd) {
    const int numButtons = 8;
    TBBUTTON tbButtons[numButtons] = {};

    // Create toolbar with larger buttons
    g_hwndToolbar = CreateWindowEx(
        0, TOOLBARCLASSNAME, nullptr,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_LIST,
        0, 0, 0, 0,
        hwnd, (HMENU)1, GetModuleHandle(nullptr), nullptr
    );

    // Set button size
    SendMessage(g_hwndToolbar, TB_SETBUTTONSIZE, 0, MAKELPARAM(24, 24));
    SendMessage(g_hwndToolbar, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS);

    tbButtons[0].iBitmap = 0;
    tbButtons[0].idCommand = 1001;
    tbButtons[0].fsState = TBSTATE_ENABLED;
    tbButtons[0].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
    tbButtons[0].iString = (INT_PTR)L"New";

    tbButtons[1].iBitmap = 1;
    tbButtons[1].idCommand = 1002;
    tbButtons[1].fsState = TBSTATE_ENABLED;
    tbButtons[1].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
    tbButtons[1].iString = (INT_PTR)L"Open";

    tbButtons[2].iBitmap = 2;
    tbButtons[2].idCommand = 1003;
    tbButtons[2].fsState = TBSTATE_ENABLED;
    tbButtons[2].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
    tbButtons[2].iString = (INT_PTR)L"Save";

    tbButtons[3].fsStyle = BTNS_SEP;

    tbButtons[4].iBitmap = 3;
    tbButtons[4].idCommand = 3001;
    tbButtons[4].fsState = TBSTATE_ENABLED;
    tbButtons[4].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
    tbButtons[4].iString = (INT_PTR)L"Add Node";

    tbButtons[5].iBitmap = 4;
    tbButtons[5].idCommand = 3002;
    tbButtons[5].fsState = TBSTATE_ENABLED;
    tbButtons[5].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
    tbButtons[5].iString = (INT_PTR)L"Remove Node";

    tbButtons[6].fsStyle = BTNS_SEP;

    tbButtons[7].iBitmap = 5;
    tbButtons[7].idCommand = 4001;
    tbButtons[7].fsState = TBSTATE_ENABLED;
    tbButtons[7].fsStyle = BTNS_BUTTON | BTNS_AUTOSIZE;
    tbButtons[7].iString = (INT_PTR)L"Word Wrap";

    SendMessage(g_hwndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(g_hwndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);
    SendMessage(g_hwndToolbar, TB_AUTOSIZE, 0, 0);
}

void CreateTreeView(HWND hwnd) {
    g_hwndTreeView = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_TREEVIEW, L"Tree View",
        WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
        0, 0, 0, 0,
        hwnd, (HMENU)2, GetModuleHandle(nullptr), nullptr
    );

    // Create image list for tree view
    g_hImageList = ImageList_Create(16, 16, ILC_COLOR32, 2, 0);

    // Add some default icons
    HICON hIconFolder = (HICON)LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 16, 16, LR_SHARED);
    HICON hIconDocument = (HICON)LoadImage(nullptr, IDI_INFORMATION, IMAGE_ICON, 16, 16, LR_SHARED);

    ImageList_AddIcon(g_hImageList, hIconFolder);
    ImageList_AddIcon(g_hImageList, hIconDocument);

    TreeView_SetImageList(g_hwndTreeView, g_hImageList, TVSIL_NORMAL);

    // Add root node - FIXED: Use non-const buffer for text
    TVINSERTSTRUCT tvInsert = {};
    tvInsert.hParent = TVI_ROOT;
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

    // Create a non-const buffer for the text
    wchar_t rootText[] = L"Root Node";
    tvInsert.item.pszText = rootText;
    tvInsert.item.iImage = 0;
    tvInsert.item.iSelectedImage = 0;

    HTREEITEM hRoot = TreeView_InsertItem(g_hwndTreeView, &tvInsert);
    g_rootNode = new TreeNode(L"Root Node", hRoot);

    // Add some sample nodes
    AddTreeNode(L"Documents", hRoot);
    AddTreeNode(L"Images", hRoot);
    AddTreeNode(L"Music", hRoot);
}

void CreateTextBox(HWND hwnd) {
    // Create container window
    g_hwndContainer = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"STATIC", nullptr,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        0, 0, 0, 0,
        hwnd, (HMENU)3, GetModuleHandle(nullptr), nullptr
    );

    // Create multiline text box
    g_hwndTextBox = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN,
        0, 0, 0, 0,
        g_hwndContainer, (HMENU)4, GetModuleHandle(nullptr), nullptr
    );

    // Create font
    g_hTextFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Consolas");

    SendMessage(g_hwndTextBox, WM_SETFONT, (WPARAM)g_hTextFont, TRUE);

    // Set limit text to 64KB
    SendMessage(g_hwndTextBox, EM_SETLIMITTEXT, 65536, 0);

    // Subclass the text box for custom handling
    g_OriginalTextBoxProc = (WNDPROC)SetWindowLongPtr(g_hwndTextBox, GWLP_WNDPROC, (LONG_PTR)TextBoxProc);

    // Set some initial text
    SetWindowText(g_hwndTextBox,
        L"Welcome to the custom text editor!\r\n"
        L"This is a multiline EDIT control with custom functionality.\r\n"
        L"\r\n"
        L"Features:\r\n"
        L"- Multi-line text editing\r\n"
        L"- Scroll bars\r\n"
        L"- Copy/Paste support\r\n"
        L"- Custom font\r\n"
        L"- Word wrap toggle\r\n"
        L"\r\n"
        L"You can type here and edit the text as needed.\r\n"
        L"Use the toolbar buttons to manage the tree view nodes on the left."
    );
}

void AddTreeNode(const std::wstring& text, HTREEITEM hParent) {
    static int nodeCount = 1;
    std::wstring nodeText = text.empty() ? L"Node " + std::to_wstring(nodeCount++) : text;

    TVINSERTSTRUCT tvInsert = {};
    tvInsert.hParent = hParent;
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

    // Create a non-const buffer for the text
    std::vector<wchar_t> textBuffer(nodeText.begin(), nodeText.end());
    textBuffer.push_back(L'\0');
    tvInsert.item.pszText = textBuffer.data();
    tvInsert.item.iImage = 1;
    tvInsert.item.iSelectedImage = 1;

    HTREEITEM hNewItem = TreeView_InsertItem(g_hwndTreeView, &tvInsert);

    // Simple implementation - add to root node's children
    TreeNode* newNode = new TreeNode(nodeText, hNewItem, g_rootNode);
    g_rootNode->children.push_back(newNode);

    TreeView_Expand(g_hwndTreeView, hParent, TVE_EXPAND);
}
void RemoveSelectedTreeNode() {
    HTREEITEM hSelected = TreeView_GetSelection(g_hwndTreeView);
    if (hSelected && hSelected != TreeView_GetRoot(g_hwndTreeView)) {
        TreeView_DeleteItem(g_hwndTreeView, hSelected);
    }
}

void ToggleWordWrap() {
    static bool wordWrap = false;
    wordWrap = !wordWrap;

    DWORD style = GetWindowLong(g_hwndTextBox, GWL_STYLE);
    if (wordWrap) {
        style &= ~WS_HSCROLL;
        style &= ~ES_AUTOHSCROLL;
    }
    else {
        style |= WS_HSCROLL;
        style |= ES_AUTOHSCROLL;
    }

    SetWindowLong(g_hwndTextBox, GWL_STYLE, style);
    SetWindowPos(g_hwndTextBox, nullptr, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // Update checkmark in menu
    HMENU hMenu = GetMenu(g_hwndMain);
    CheckMenuItem(hMenu, 4001, MF_BYCOMMAND | (wordWrap ? MF_CHECKED : MF_UNCHECKED));
}

void UpdateLayout(int width, int height) {
    int toolbarHeight = 0;
    if (g_hwndToolbar && IsWindowVisible(g_hwndToolbar)) {
        RECT rcToolbar;
        GetWindowRect(g_hwndToolbar, &rcToolbar);
        toolbarHeight = rcToolbar.bottom - rcToolbar.top;
        SetWindowPos(g_hwndToolbar, nullptr, 0, 0, width, toolbarHeight, SWP_NOZORDER);
    }

    int treeWidth = 250; // Fixed width for tree view
    int containerWidth = width - treeWidth;

    if (g_hwndTreeView) {
        SetWindowPos(g_hwndTreeView, nullptr, 0, toolbarHeight, treeWidth, height - toolbarHeight, SWP_NOZORDER);
    }

    if (g_hwndContainer) {
        SetWindowPos(g_hwndContainer, nullptr, treeWidth, toolbarHeight, containerWidth, height - toolbarHeight, SWP_NOZORDER);

        if (g_hwndTextBox) {
            RECT rcContainer;
            GetClientRect(g_hwndContainer, &rcContainer);
            SetWindowPos(g_hwndTextBox, nullptr, 2, 2, rcContainer.right - 4, rcContainer.bottom - 4, SWP_NOZORDER);
        }
    }
}

// Custom text box window procedure
LRESULT CALLBACK TextBoxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN:
        // Handle Ctrl+A for select all
        if (wParam == 'A' && GetKeyState(VK_CONTROL) & 0x8000) {
            SendMessage(hwnd, EM_SETSEL, 0, -1);
            return 0;
        }
        // Handle Ctrl+Z for undo (standard EDIT control supports this)
        break;

    case WM_CHAR:
        // Custom character handling if needed
        break;

    case WM_MOUSEWHEEL:
        // Custom mouse wheel handling
        break;
    }

    // Call original window procedure
    return CallWindowProc(g_OriginalTextBoxProc, hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateMenuBar(hwnd);
        CreateToolbar(hwnd);
        CreateTreeView(hwnd);
        CreateTextBox(hwnd);
        break;

    case WM_SIZE:
        UpdateLayout(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1004: // Exit
            PostQuitMessage(0);
            break;

        case 2001: // Undo
            SendMessage(g_hwndTextBox, EM_UNDO, 0, 0);
            break;

        case 2002: // Cut
            SendMessage(g_hwndTextBox, WM_CUT, 0, 0);
            break;

        case 2003: // Copy
            SendMessage(g_hwndTextBox, WM_COPY, 0, 0);
            break;

        case 2004: // Paste
            SendMessage(g_hwndTextBox, WM_PASTE, 0, 0);
            break;

        case 2005: // Select All
            SendMessage(g_hwndTextBox, EM_SETSEL, 0, -1);
            break;

        case 3001: // Add Node
            AddTreeNode(L"");
            break;

        case 3002: // Remove Node
            RemoveSelectedTreeNode();
            break;

        case 3003: // Clear All
        {
            // Keep root node, remove all children
            for (auto child : g_rootNode->children) {
                delete child;
            }
            g_rootNode->children.clear();
            TreeView_DeleteAllItems(g_hwndTreeView);

            // Recreate root node - FIXED: Added braces to create a scope
            TVINSERTSTRUCT tvInsert = {};
            tvInsert.hParent = TVI_ROOT;
            tvInsert.hInsertAfter = TVI_LAST;
            tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

            wchar_t rootText[] = L"Root Node";
            tvInsert.item.pszText = rootText;
            tvInsert.item.iImage = 0;
            tvInsert.item.iSelectedImage = 0;

            HTREEITEM hRoot = TreeView_InsertItem(g_hwndTreeView, &tvInsert);
            g_rootNode->hItem = hRoot;
        }
        break;

        case 4001: // Word Wrap
            ToggleWordWrap();
            break;

        case 4002: // Font
            // Font dialog would go here
            MessageBox(hwnd, L"Font dialog would open here", L"Font", MB_OK);
            break;
        }
        break;

    case WM_NOTIFY:
    {
        LPNMHDR nmhdr = (LPNMHDR)lParam;
        if (nmhdr->code == TTN_GETDISPINFO) {
            // Tooltip handling
            LPNMTTDISPINFO lpnmtdi = (LPNMTTDISPINFO)lParam;
            // Add tooltip text here if needed
        }
    }
    break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        // Cleanup
        if (g_hImageList) ImageList_Destroy(g_hImageList);
        if (g_rootNode) delete g_rootNode;
        if (g_hTextFont) DeleteObject(g_hTextFont);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}