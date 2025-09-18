#pragma once
class IMenuEventHandler {
public:
    virtual ~IMenuEventHandler() = default;
    virtual void OnFileNew() = 0;
    virtual void OnFileExit() = 0;
};