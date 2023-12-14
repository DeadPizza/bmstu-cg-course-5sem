#include "states/AppStateMachine.h"
#include "managers/InputManager.h"

void AppStateMachine::NewFrame()
{
    if (m_state == AppState::MENU_EDITING_RENDER_RESET)
    {
        m_state = AppState::MENU_EDITING;
    }

    if (InputManager::GetKeyDown(GLFW_KEY_F3))
    {
        if (m_state == AppState::MENU_EDITING)
        {
            m_state = AppState::FREE_CAM;
        }
        else if(m_state == AppState::FREE_CAM)
        {
            m_state = AppState::MENU_EDITING;
        }
    }

    if(m_state == AppState::ANIMATOR_SETTING_KEYFRAMES && InputManager::GetKeyDown(GLFW_KEY_X))
    {
        m_state = AppState::MENU_EDITING;
    }
}

void AppStateMachine::ResetRender()
{
    if (m_state == AppState::MENU_EDITING)
    {
        m_state = AppState::MENU_EDITING_RENDER_RESET;
    }
}

bool AppStateMachine::isCamUpdate()
{
    return m_state == AppState::FREE_CAM || m_state == AppState::ANIMATOR_SETTING_KEYFRAMES;
}

bool AppStateMachine::isMenuUpdate()
{
    return m_state == AppState::MENU_EDITING || m_state == AppState::MENU_EDITING_RENDER_RESET;
}

bool AppStateMachine::isResetRender()
{
    return m_state == AppState::MENU_EDITING_RENDER_RESET;
}
