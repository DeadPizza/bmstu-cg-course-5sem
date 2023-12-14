#pragma once

#include <vector>
#include <memory>

#include "ui/UIWindow.h"

class UIComposite : public UIWindow
{
    using Iterator = std::vector<std::unique_ptr<UIWindow>>::iterator;
public:
    UIComposite(std::vector<std::unique_ptr<UIWindow>> &&ui_windows);
    void Init(Application &) override;
    bool Update(Application &) override;

    void SaveState(std::ostream &f) override;
    void RestoreState(std::istream &f) override;

    Iterator begin() override;
    Iterator end() override;

private:
    std::vector<std::unique_ptr<UIWindow>> m_ui_windows;
};

using UIVectorOfPtrs = std::vector<UIWindowPtr>;