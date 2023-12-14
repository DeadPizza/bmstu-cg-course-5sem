#pragma once

#include "ui/UIWindow.h"
#include "scene/SceneObjectMenu.h"

class UISceneEditor : public UIWindow
{
public:
    void Init(Application &app) override;
    bool Update(Application &app) override;

private:
    SceneObjectMenu m_menu_visitor;

    int m_cur_obj;
    
};