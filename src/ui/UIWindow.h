#pragma once

#include <vector>
#include <memory>
#include <ostream>
#include <istream>

class Application;

class UIWindow
{
    using Iterator = std::vector<std::unique_ptr<UIWindow>>::iterator;
public:
    virtual ~UIWindow() = default;

    virtual void Init(Application &) = 0;
    virtual bool Update(Application &) = 0;

    virtual void SaveState(std::ostream &) {};
    virtual void RestoreState(std::istream &) {};

    virtual Iterator begin() { return Iterator(); }
    virtual Iterator end() { return Iterator(); }
};

using UIWindowPtr = std::unique_ptr<UIWindow>;