#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

// Enums for input types and keycodes
typedef enum {
    // Application events
    WING_FIRSTEVENT = 0,

    WING_QUIT,

    WING_KEYDOWN,
    WING_KEYUP,

    // Mouse events
    WING_MOUSEMOTION,
    WING_MOUSEBUTTONDOWN,
    WING_MOUSEBUTTONUP,
    WING_MOUSEWHEEL,

    // Touch events
    WING_FINGERDOWN,
    WING_FINGERUP,
    WING_FINGERMOTION,

    // Add other event types as needed

    WING_LASTEVENT
} WING_InputType;

typedef enum {
    WING_K_UNKNOWN = 0,

    // Letters
    WING_K_A,
    WING_K_B,
    WING_K_C,
    WING_K_D,
    WING_K_E,
    WING_K_F,
    WING_K_G,
    WING_K_H,
    WING_K_I,
    WING_K_J,
    WING_K_K,
    WING_K_L,
    WING_K_M,
    WING_K_N,
    WING_K_O,
    WING_K_P,
    WING_K_Q,
    WING_K_R,
    WING_K_S,
    WING_K_T,
    WING_K_U,
    WING_K_V,
    WING_K_W,
    WING_K_X,
    WING_K_Y,
    WING_K_Z,

    // Numbers
    WING_K_0,
    WING_K_1,
    WING_K_2,
    WING_K_3,
    WING_K_4,
    WING_K_5,
    WING_K_6,
    WING_K_7,
    WING_K_8,
    WING_K_9,

    // Function keys
    WING_K_F1,
    WING_K_F2,
    WING_K_F3,
    WING_K_F4,
    WING_K_F5,
    WING_K_F6,
    WING_K_F7,
    WING_K_F8,
    WING_K_F9,
    WING_K_F10,
    WING_K_F11,
    WING_K_F12,

    // Control keys
    WING_K_RETURN,
    WING_K_ESCAPE,
    WING_K_BACKSPACE,
    WING_K_TAB,
    WING_K_SPACE,
    WING_K_DELETE,

    // Arrow keys
    WING_K_RIGHT,
    WING_K_LEFT,
    WING_K_DOWN,
    WING_K_UP,

    // Modifier keys
    WING_K_LSHIFT,
    WING_K_RSHIFT,
    WING_K_LCTRL,
    WING_K_RCTRL,
    WING_K_LALT,
    WING_K_RALT,

    // Punctuation and symbols
    WING_K_EXCLAIM,
    WING_K_QUOTEDBL,
    WING_K_HASH,
    WING_K_PERCENT,
    WING_K_DOLLAR,
    WING_K_AMPERSAND,
    WING_K_QUOTE,
    WING_K_LEFTPAREN,
    WING_K_RIGHTPAREN,
    WING_K_ASTERISK,
    WING_K_PLUS,
    WING_K_COMMA,
    WING_K_MINUS,
    WING_K_PERIOD,
    WING_K_SLASH,
    WING_K_COLON,
    WING_K_SEMICOLON,
    WING_K_LESS,
    WING_K_EQUALS,
    WING_K_GREATER,
    WING_K_QUESTION,
    WING_K_AT,
    WING_K_LEFTBRACKET,
    WING_K_BACKSLASH,
    WING_K_RIGHTBRACKET,
    WING_K_CARET,
    WING_K_UNDERSCORE,
    WING_K_BACKQUOTE,

    // Keypad keys
    WING_K_KP_0,
    WING_K_KP_1,
    WING_K_KP_2,
    WING_K_KP_3,
    WING_K_KP_4,
    WING_K_KP_5,
    WING_K_KP_6,
    WING_K_KP_7,
    WING_K_KP_8,
    WING_K_KP_9,
    WING_K_KP_PERIOD,
    WING_K_KP_DIVIDE,
    WING_K_KP_MULTIPLY,
    WING_K_KP_MINUS,
    WING_K_KP_PLUS,
    WING_K_KP_ENTER,
    WING_K_KP_EQUALS,

    // Other keys
    WING_K_INSERT,
    WING_K_HOME,
    WING_K_END,
    WING_K_PAGEUP,
    WING_K_PAGEDOWN,

    // Media keys
    WING_K_MUTE,
    WING_K_VOLUMEUP,
    WING_K_VOLUMEDOWN,

    // Add other keys as needed

    WING_K_LAST
} WING_Keycode;

// Enums for initialization flags
typedef enum {
    WING_INIT_TIMER = 0x00000001,
    WING_INIT_AUDIO = 0x00000010,
    WING_INIT_VIDEO = 0x00000020,
    // Add other init flags as needed
    WING_INIT_EVERYTHING = 0x0010FFFF
} WING_InitFlags;

// TODO: send to window class
// Enums for window flags
typedef enum {
    WING_WINDOW_FULLSCREEN = 0x00000001,
    WING_WINDOW_SHOWN = 0x00000004,
    // Add other window flags as needed
} WING_WindowFlags;

// Enums for window position
typedef enum {
    WING_WINDOWPOS_UNDEFINED = 0,
    WING_WINDOWPOS_CENTERED = 1
} WING_WindowPosition;


// EventData class
class EventData {
public:
    WING_InputType type;
    WING_Keycode keycode; // For key events; otherwise, set to WING_K_UNKNOWN

    EventData() : type(WING_FIRSTEVENT), keycode(WING_K_UNKNOWN) {}
};