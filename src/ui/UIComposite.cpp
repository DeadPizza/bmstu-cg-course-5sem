#include "ui/UIComposite.h"

UIComposite::UIComposite(std::vector<std::unique_ptr<UIWindow>> &&ui_windows)
    : m_ui_windows(std::move(ui_windows))
{}

void UIComposite::Init(Application &app)
{
    for (auto &stored_window : m_ui_windows)
    {
        stored_window->Init(app);
    }
}

bool UIComposite::Update(Application &app)
{
    bool changed = false;
    for (auto &stored_window : m_ui_windows)
    {
        changed |= stored_window->Update(app);
    }
    return changed;
}

void UIComposite::SaveState(std::ostream &f)
{
    for (auto &stored_window : m_ui_windows)
    {
        stored_window->SaveState(f);
    }
}

void UIComposite::RestoreState(std::istream &f)
{
    for (auto &stored_window : m_ui_windows)
    {
        stored_window->RestoreState(f);
    }
}

UIComposite::Iterator UIComposite::begin()
{
    return m_ui_windows.begin();
}

UIComposite::Iterator UIComposite::end()
{
    return m_ui_windows.end();
}