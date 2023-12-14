#pragma once

enum class AppState
{
    FREE_CAM,
    MENU_EDITING,
    MENU_EDITING_RENDER_RESET,
    ANIMATOR_SETTING_KEYFRAMES
};

class AppStateMachine
{
public:
    AppStateMachine()
    : m_state(AppState::MENU_EDITING)
    {}
    
    void NewFrame();
    void ResetRender();

    bool isCamUpdate();
    bool isMenuUpdate();
    bool isResetRender();
    
private:
    AppState m_state;
};