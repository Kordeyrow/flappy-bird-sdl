#pragma once

#include <dll/export_def.h>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

// Enums for input types and keycodes
typedef enum {
    // Application events
    SHP_FIRSTEVENT = 0,

    SHP_QUIT,

    SHP_KEYDOWN,
    SHP_KEYUP,

    // Mouse events
    SHP_MOUSEMOTION,
    SHP_MOUSEBUTTONDOWN,
    SHP_MOUSEBUTTONUP,
    SHP_MOUSEWHEEL,

    // Touch events
    SHP_FINGERDOWN,
    SHP_FINGERUP,
    SHP_FINGERMOTION,

    // Add other event types as needed

    SHP_LASTEVENT
} SHP_InputType;

typedef enum {
    SHP_K_UNKNOWN = 0,

    // Letters
    SHP_K_A,
    SHP_K_B,
    SHP_K_C,
    SHP_K_D,
    SHP_K_E,
    SHP_K_F,
    SHP_K_G,
    SHP_K_H,
    SHP_K_I,
    SHP_K_J,
    SHP_K_K,
    SHP_K_L,
    SHP_K_M,
    SHP_K_N,
    SHP_K_O,
    SHP_K_P,
    SHP_K_Q,
    SHP_K_R,
    SHP_K_S,
    SHP_K_T,
    SHP_K_U,
    SHP_K_V,
    SHP_K_W,
    SHP_K_X,
    SHP_K_Y,
    SHP_K_Z,

    // Numbers
    SHP_K_0,
    SHP_K_1,
    SHP_K_2,
    SHP_K_3,
    SHP_K_4,
    SHP_K_5,
    SHP_K_6,
    SHP_K_7,
    SHP_K_8,
    SHP_K_9,

    // Function keys
    SHP_K_F1,
    SHP_K_F2,
    SHP_K_F3,
    SHP_K_F4,
    SHP_K_F5,
    SHP_K_F6,
    SHP_K_F7,
    SHP_K_F8,
    SHP_K_F9,
    SHP_K_F10,
    SHP_K_F11,
    SHP_K_F12,

    // Control keys
    SHP_K_RETURN,
    SHP_K_ESCAPE,
    SHP_K_BACKSPACE,
    SHP_K_TAB,
    SHP_K_SPACE,
    SHP_K_DELETE,

    // Arrow keys
    SHP_K_RIGHT,
    SHP_K_LEFT,
    SHP_K_DOWN,
    SHP_K_UP,

    // Modifier keys
    SHP_K_LSHIFT,
    SHP_K_RSHIFT,
    SHP_K_LCTRL,
    SHP_K_RCTRL,
    SHP_K_LALT,
    SHP_K_RALT,

    // Punctuation and symbols
    SHP_K_EXCLAIM,
    SHP_K_QUOTEDBL,
    SHP_K_HASH,
    SHP_K_PERCENT,
    SHP_K_DOLLAR,
    SHP_K_AMPERSAND,
    SHP_K_QUOTE,
    SHP_K_LEFTPAREN,
    SHP_K_RIGHTPAREN,
    SHP_K_ASTERISK,
    SHP_K_PLUS,
    SHP_K_COMMA,
    SHP_K_MINUS,
    SHP_K_PERIOD,
    SHP_K_SLASH,
    SHP_K_COLON,
    SHP_K_SEMICOLON,
    SHP_K_LESS,
    SHP_K_EQUALS,
    SHP_K_GREATER,
    SHP_K_QUESTION,
    SHP_K_AT,
    SHP_K_LEFTBRACKET,
    SHP_K_BACKSLASH,
    SHP_K_RIGHTBRACKET,
    SHP_K_CARET,
    SHP_K_UNDERSCORE,
    SHP_K_BACKQUOTE,

    // Keypad keys
    SHP_K_KP_0,
    SHP_K_KP_1,
    SHP_K_KP_2,
    SHP_K_KP_3,
    SHP_K_KP_4,
    SHP_K_KP_5,
    SHP_K_KP_6,
    SHP_K_KP_7,
    SHP_K_KP_8,
    SHP_K_KP_9,
    SHP_K_KP_PERIOD,
    SHP_K_KP_DIVIDE,
    SHP_K_KP_MULTIPLY,
    SHP_K_KP_MINUS,
    SHP_K_KP_PLUS,
    SHP_K_KP_ENTER,
    SHP_K_KP_EQUALS,

    // Other keys
    SHP_K_INSERT,
    SHP_K_HOME,
    SHP_K_END,
    SHP_K_PAGEUP,
    SHP_K_PAGEDOWN,

    // Media keys
    SHP_K_MUTE,
    SHP_K_VOLUMEUP,
    SHP_K_VOLUMEDOWN,

    // Add other keys as needed

    SHP_K_LAST
} SHP_Keycode;

// Enums for initialization flags
typedef enum {
    SHP_INIT_TIMER = 0x00000001,
    SHP_INIT_AUDIO = 0x00000010,
    SHP_INIT_VIDEO = 0x00000020,
    // Add other init flags as needed
    SHP_INIT_EVERYTHING = 0x0010FFFF
} SHP_InitFlags;

// Enums for window flags
typedef enum {
    SHP_WINDOW_FULLSCREEN = 0x00000001,
    SHP_WINDOW_SHOWN = 0x00000004,
    // Add other window flags as needed
} SHP_WindowFlags;

// Enums for window position
typedef enum {
    SHP_WINDOWPOS_UNDEFINED = 0,
    SHP_WINDOWPOS_CENTERED = 1
} SHP_WindowPosition;

// EventData class
class EventData {
public:
    SHP_InputType type;
    SHP_Keycode keycode; // For key events; otherwise, set to SHP_K_UNKNOWN

    EventData() : type(SHP_FIRSTEVENT), keycode(SHP_K_UNKNOWN) {}
};

// Initialization parameters
struct InitParams {
    SHP_InitFlags init_flags;
    std::string window_title;
    SHP_WindowPosition window_x;
    SHP_WindowPosition window_y;
    int window_w;
    int window_h;
    SHP_WindowFlags window_flags;

    InitParams()
        : init_flags(SHP_INIT_VIDEO),
        window_title("Game"),
        window_x(SHP_WINDOWPOS_CENTERED),
        window_y(SHP_WINDOWPOS_CENTERED),
        window_w(800),
        window_h(600),
        window_flags(SHP_WINDOW_SHOWN) {}
};

