//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Window.hpp"

namespace oe::Input
{
    enum Key
    {
        Q = GLFW_KEY_Q,
        W = GLFW_KEY_W,
        E = GLFW_KEY_E,
        R = GLFW_KEY_R,
        T = GLFW_KEY_T,
        Y = GLFW_KEY_Y,
        U = GLFW_KEY_U,
        I = GLFW_KEY_I,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        A = GLFW_KEY_A,
        S = GLFW_KEY_S,
        D = GLFW_KEY_D,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        Z = GLFW_KEY_Z,
        X = GLFW_KEY_X,
        C = GLFW_KEY_C,
        V = GLFW_KEY_V,
        B = GLFW_KEY_B,
        N = GLFW_KEY_N,
        M = GLFW_KEY_M,
        ESC = GLFW_KEY_ESCAPE,
        SPACE = GLFW_KEY_SPACE,
        ENTER = GLFW_KEY_ENTER,
        LEFT_CTRL = GLFW_KEY_LEFT_CONTROL,
        RIGHT_CTRL = GLFW_KEY_RIGHT_CONTROL,
    };

    enum Action : uint8_t
    {
        PRESS = GLFW_PRESS,
        RELEASE = GLFW_RELEASE
    };

    enum Button : uint8_t
    {
        LEFT = GLFW_MOUSE_BUTTON_LEFT,
        RIGHT = GLFW_MOUSE_BUTTON_RIGHT
    };

    enum InputMode
    {
        CURSOR = GLFW_CURSOR
    };

    enum InputValue
    {
        CURSOR_NORMAL = GLFW_CURSOR_NORMAL,
        CURSOR_DISABLED = GLFW_CURSOR_DISABLED
    };

    class Command
    {
      public:
        virtual ~Command() = default;
        virtual void Execute() = 0;
        virtual void Undo() = 0;
    };

    bool IsKey(Action action, Key key);
    bool IsMouseButton(Action action, Button button);
    void SetMode(InputMode mode, InputValue value);
    int GetMode(InputMode mode);
} // namespace oe::Input
