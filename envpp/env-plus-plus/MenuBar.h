//#ifndef MENUBAR_H
//#define MENUBAR_H
//
//#include <windows.h>
//#include <functional>
//
//class MenuBar {
//public:
//    MenuBar();
//    ~MenuBar();
//
//    HMENU GetHandle() const { return hMenu; }
//
//    void SetOnNew(std::function<void()> handler) { onNew = handler; }
//    void SetOnExit(std::function<void()> handler) { onExit = handler; }
//
//    void HandleCommand(WORD);
//
//private:
//    HMENU hMenu;
//    enum MenuIds { ID_NEW = 1, ID_EXIT };
//
//    std::function<void()> onNew;
//    std::function<void()> onExit;
//};
//
//#endif
