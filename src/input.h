#ifndef CEDAR_INPUT_H
#define CEDAR_INPUT_H

namespace Cedar
{
    enum class Key {
        Backspace,
        Tab,
        Enter,
        Shift,
        Control,
        Alt,
        Pause,
        Caps_Lock,
        Escape,
        Space,
        Page_Up,
        Page_Down,
        End,
        Home,
        Left_Arrow,
        Up_Arrow,
        Right_Arrow,
        Down_Arrow,
        Select,
        Print,
        Execute,
        Print_Screen,
        Insert,
        Delete,
        Help,

        D0,
        D1,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        D8,
        D9,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        
        Left_Windows,
        Right_Windows,
        Applications,
        Sleep,

        Numpad_0,
        Numpad_1,
        Numpad_2,
        Numpad_3,
        Numpad_4,
        Numpad_5,
        Numpad_6,
        Numpad_7,
        Numpad_8,
        Numpad_9,

        Multiply,
        Add,
        Separator,
        Subtract,
        Decimal,
        Divide,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,

        Num_Lock,
        Scroll_Lock,
        Left_Shift,
        Right_Shift,
        Left_Control,
        Right_Control,
        Left_Alt,
        Right_Alt,

        Volume_Mute,
        Volume_Down,
        Volume_Up,

        Semicolon,     /* ; */
        Equal,         /* = */
        Comma,         /* , */
        Minus,         /* - */
        Period,        /* . */
        Slash,         /* / */
        Grave_Accent,  /* ` */
        Open_Bracket,  /* [ */
        Backslash,     /* \ */
        Close_Bracket, /* ] */
        Apostrophe,    /* ' */
    };
}

#endif // CEDAR_INPUT_H