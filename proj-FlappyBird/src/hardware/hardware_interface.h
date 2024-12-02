// HardwareInterface.h

#ifndef HARDWAREINTERFACE_H
#define HARDWAREINTERFACE_H

#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <iostream>
#include <string>

// Enums for input types and keycodes
typedef enum {
    // Application events
    SHP_FIRSTEVENT = 0,

    SHP_QUIT,

    SHP_APP_TERMINATING,
    SHP_APP_LOWMEMORY,
    SHP_APP_WILLENTERBACKGROUND,
    SHP_APP_DIDENTERBACKGROUND,
    SHP_APP_WILLENTERFOREGROUND,
    SHP_APP_DIDENTERFOREGROUND,

    SHP_LOCALECHANGED,

    // Display events
    SHP_DISPLAYEVENT,

    // Window events
    SHP_WINDOWEVENT,
    SHP_SYSWMEVENT,

    // Keyboard events
    SHP_KEYDOWN,
    SHP_KEYUP,
    SHP_TEXTEDITING,
    SHP_TEXTINPUT,
    SHP_KEYMAPCHANGED,
    SHP_TEXTEDITING_EXT,

    // Mouse events
    SHP_MOUSEMOTION,
    SHP_MOUSEBUTTONDOWN,
    SHP_MOUSEBUTTONUP,
    SHP_MOUSEWHEEL,

    // Joystick events
    SHP_JOYAXISMOTION,
    SHP_JOYBALLMOTION,
    SHP_JOYHATMOTION,
    SHP_JOYBUTTONDOWN,
    SHP_JOYBUTTONUP,
    SHP_JOYDEVICEADDED,
    SHP_JOYDEVICEREMOVED,
    SHP_JOYBATTERYUPDATED,

    // Game controller events
    SHP_CONTROLLERAXISMOTION,
    SHP_CONTROLLERBUTTONDOWN,
    SHP_CONTROLLERBUTTONUP,
    SHP_CONTROLLERDEVICEADDED,
    SHP_CONTROLLERDEVICEREMOVED,
    SHP_CONTROLLERDEVICEREMAPPED,
    SHP_CONTROLLERTOUCHPADDOWN,
    SHP_CONTROLLERTOUCHPADMOTION,
    SHP_CONTROLLERTOUCHPADUP,
    SHP_CONTROLLERSENSORUPDATE,
    SHP_CONTROLLERSTEAMHANDLEUPDATED,

    // Touch events
    SHP_FINGERDOWN,
    SHP_FINGERUP,
    SHP_FINGERMOTION,

    // Gesture events
    SHP_DOLLARGESTURE,
    SHP_DOLLARRECORD,
    SHP_MULTIGESTURE,

    // Clipboard events
    SHP_CLIPBOARDUPDATE,

    // Drag and drop events
    SHP_DROPFILE,
    SHP_DROPTEXT,
    SHP_DROPBEGIN,
    SHP_DROPCOMPLETE,

    // Audio hotplug events
    SHP_AUDIODEVICEADDED,
    SHP_AUDIODEVICEREMOVED,

    // Sensor events
    SHP_SENSORUPDATE,

    // Render events
    SHP_RENDER_TARGETS_RESET,
    SHP_RENDER_DEVICE_RESET,

    // Internal events
    SHP_POLLSENTINEL,

    // User events
    SHP_USEREVENT,

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

// Enums for initialization flags, wrapping SDL flags
typedef enum {
    SHP_INIT_TIMER = 0x00000001,
    SHP_INIT_AUDIO = 0x00000010,
    SHP_INIT_VIDEO = 0x00000020,
    SHP_INIT_JOYSTICK = 0x00000200,
    SHP_INIT_HAPTIC = 0x00001000,
    SHP_INIT_GAMECONTROLLER = 0x00002000,
    SHP_INIT_EVENTS = 0x00004000,
    SHP_INIT_NOPARACHUTE = 0x00100000,
    SHP_INIT_EVERYTHING = 0x0010FFFF
} SHP_InitFlags;

// Enums for window flags, wrapping SDL window flags
typedef enum {
    SHP_WINDOW_FULLSCREEN = 0x00000001,
    SHP_WINDOW_OPENGL = 0x00000002,
    SHP_WINDOW_SHOWN = 0x00000004,
    SHP_WINDOW_HIDDEN = 0x00000008,
    SHP_WINDOW_BORDERLESS = 0x00000010,
    SHP_WINDOW_RESIZABLE = 0x00000020,
    SHP_WINDOW_MINIMIZED = 0x00000040,
    SHP_WINDOW_MAXIMIZED = 0x00000080,
    SHP_WINDOW_INPUT_GRABBED = 0x00000100,
    SHP_WINDOW_INPUT_FOCUS = 0x00000200,
    SHP_WINDOW_MOUSE_FOCUS = 0x00000400,
    SHP_WINDOW_FULLSCREEN_DESKTOP = 0x00001001, // SDL_WINDOW_FULLSCREEN_DESKTOP
    SHP_WINDOW_FOREIGN = 0x00000800,
    SHP_WINDOW_ALLOW_HIGHDPI = 0x00002000,
    SHP_WINDOW_MOUSE_CAPTURE = 0x00004000,
    SHP_WINDOW_ALWAYS_ON_TOP = 0x00008000,
    SHP_WINDOW_SKIP_TASKBAR = 0x00010000,
    SHP_WINDOW_UTILITY = 0x00020000,
    SHP_WINDOW_TOOLTIP = 0x00040000,
    SHP_WINDOW_POPUP_MENU = 0x00080000,
    SHP_WINDOW_VULKAN = 0x10000000,
    SHP_WINDOW_METAL = 0x20000000
} SHP_WindowFlags;

// EventData class
class EventData {
public:
    SHP_InputType type;
    SHP_Keycode keycode; // For key events; for other events, this can be ignored or set to SHP_K_UNKNOWN

    EventData() : type(SHP_FIRSTEVENT), keycode(SHP_K_UNKNOWN) {}
};

class HardwareInterface {
public:
    HardwareInterface();
    ~HardwareInterface();

    bool init(SHP_InitFlags initFlags, const std::string& windowTitle, int windowX, int windowY, int windowW, int windowH, SHP_WindowFlags windowFlags);
    void shutdown();
    bool PopEvent(EventData& eventData);

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    int _window_w;
    int _window_h;

    // Map SDL event types to SHP_InputType
    std::map<Uint32, SHP_InputType> input_type_from_sdl_event;
    // Map SDL_Keycode to SHP_Keycode
    std::map<SDL_Keycode, SHP_Keycode> keycode_from_sdl_keycode;

    // Initialization of maps
    void initInputMappings();
};

#endif // HARDWAREINTERFACE_H
