#include "managers/InputManager.h"
#include <cstring>

bool InputManager::key_down[GLFW_KEY_LAST + 1] = { 0 };
bool InputManager::key_hold[GLFW_KEY_LAST + 1] = { 0 };
bool InputManager::key_up[GLFW_KEY_LAST + 1] = { 0 };

bool InputManager::mouse_down[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
bool InputManager::mouse_hold[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
bool InputManager::mouse_up[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };

float InputManager::prev_mx = 0, InputManager::cur_mx = 0;
float InputManager::prev_my = 0, InputManager::cur_my = 0;
float InputManager::mouse_sensivity = 1;

void InputManager::Init(GLFWwindow *window_)
{

    static const GLFWkeyfun key_callback_ptr = key_callback;
    static GLFWkeyfun prev_KeyCallback = glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            key_callback_ptr(window, key, scancode, action, mods);
            if (prev_KeyCallback)
                prev_KeyCallback(window, key, scancode, action, mods);
        });


    static GLFWwindowfocusfun prev_FocusCallback = glfwSetWindowFocusCallback(window_,
        [](GLFWwindow *window, int focused) {
            if (prev_FocusCallback)
                prev_FocusCallback(window, focused);
        });

    static const GLFWmousebuttonfun mouse_button_callback_ptr = mouse_button_callback;

    static GLFWmousebuttonfun prev_MouseButtonCallback = glfwSetMouseButtonCallback(window_,
        [](GLFWwindow *window, int button, int action, int mods) {
            mouse_button_callback_ptr(window, button, action, mods);
            if (prev_MouseButtonCallback)
                prev_MouseButtonCallback(window, button, action, mods);
        });


    static const GLFWcursorposfun mouse_pos_callback_ptr = mouse_pos_callback;
    static GLFWcursorposfun prev_CursorPosCallback = glfwSetCursorPosCallback(window_,
        [](GLFWwindow *window, double xpos, double ypos) {
            mouse_pos_callback_ptr(window, xpos, ypos);
            if (prev_CursorPosCallback)
                prev_CursorPosCallback(window, xpos, ypos);
        });

    double tx, ty;
    glfwGetCursorPos(window_, &tx, &ty);
    cur_mx = prev_mx = tx;
    cur_my = prev_my = ty;
}

void InputManager::PostInit()
{
}

void InputManager::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    switch (action)
    {
        case GLFW_PRESS:
        {
            key_down[key] = true;
            key_hold[key] = true;
            // spdlog::debug("button press event: {}", key);
        }
        break;

        case GLFW_RELEASE:
        {
            key_up[key] = true;
            key_hold[key] = false;
            // spdlog::debug("button release event: {}", key);
        }
        break;

        default:
            break;
    }
}

void InputManager::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
        {
            mouse_down[button] = true;
            mouse_hold[button] = true;
            // spdlog::debug("mouse press event: {}", button);
        }
        break;

        case GLFW_RELEASE:
        {
            mouse_up[button] = true;
            mouse_hold[button] = false;
            // spdlog::debug("mouse release event: {}", button);
        }
        break;

        default:
            break;
    }
}

void InputManager::mouse_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    cur_mx = xpos;
    cur_my = ypos;
}

void InputManager::ResetAllActive()
{
    memset(key_down, 0, sizeof(key_down));
    memset(key_up, 0, sizeof(key_up));
    memset(key_hold, 0, sizeof(key_hold));
    memset(mouse_down, 0, sizeof(mouse_down));
    memset(mouse_up, 0, sizeof(mouse_up));
    memset(mouse_hold, 0, sizeof(mouse_hold));
}

void InputManager::OnFrameEnd()
{
    for (size_t i = 0; i < GLFW_KEY_LAST + 1; ++i)
    {
        key_down[i] = 0;
        key_up[i] = 0;
    }
    memset(mouse_down, 0, sizeof(mouse_down));
    memset(mouse_up, 0, sizeof(mouse_up));

    prev_mx = cur_mx;
    prev_my = cur_my;
}