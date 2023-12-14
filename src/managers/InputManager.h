#pragma once

#include <GLFW/glfw3.h>

#include <cassert>

class InputManager
{
    friend class Application;
public:
    static bool GetKeyDown(int keycode)
    {
        assert(keycode <= GLFW_KEY_LAST);
        return key_down[keycode];
    }

    static bool GetKeyUp(int keycode)
    {
        assert(keycode <= GLFW_KEY_LAST);
        return key_up[keycode];
    }

    static bool GetKey(int keycode)
    {
        assert(keycode <= GLFW_KEY_LAST);
        return key_hold[keycode];
    }

    static bool GetMouseButtonDown(int keycode)
    {
        assert(keycode <= GLFW_MOUSE_BUTTON_LAST);
        return mouse_down[keycode];
    }

    static bool GetMouseButtonUp(int keycode)
    {
        assert(keycode <= GLFW_MOUSE_BUTTON_LAST);
        return mouse_up[keycode];
    }

    static bool GetMouseButton(int keycode)
    {
        assert(keycode <= GLFW_MOUSE_BUTTON_LAST);
        return mouse_hold[keycode];
    }

    static float GetAxisHorizontal()
    {
        return key_hold[GLFW_KEY_D] - key_hold[GLFW_KEY_A];
    }

    static float GetAxisVertical()
    {
        return key_hold[GLFW_KEY_W] - key_hold[GLFW_KEY_S];
    }

    static float GetMouseHorizontal()
    {
        return (cur_mx - prev_mx) * mouse_sensivity;
    }

    static float GetMouseVertical()
    {
        return (cur_my - prev_my) * mouse_sensivity;
    }

    static float mouse_sensivity;
private:
    static void Init(GLFWwindow *window_);
    static void PostInit();

    static void ResetAllActive();
    static void OnFrameEnd();
private:
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos);

    static bool key_down[GLFW_KEY_LAST + 1];
    static bool key_hold[GLFW_KEY_LAST + 1];
    static bool key_up[GLFW_KEY_LAST + 1];

    static bool mouse_down[GLFW_MOUSE_BUTTON_LAST + 1];
    static bool mouse_hold[GLFW_MOUSE_BUTTON_LAST + 1];
    static bool mouse_up[GLFW_MOUSE_BUTTON_LAST + 1];

    static float prev_mx, cur_mx;
    static float prev_my, cur_my;
};