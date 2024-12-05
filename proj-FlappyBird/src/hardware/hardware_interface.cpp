// HardwareInterface.cpp

#include "hardware_interface.h"

#define MAP_KEYCODE(sdlk, shpk) { SDLK_##sdlk, SHP_K_##shpk }

HardwareInterface::HardwareInterface()
    : _sdl_window(nullptr), _renderer(nullptr), _window_w(0), _window_h(0) {
    // Initialize the input mappings
    initInputMappings();
}

HardwareInterface::~HardwareInterface() {
    shutdown();
}

bool HardwareInterface::init(SHP_InitFlags initFlags, const std::string& windowTitle, int windowX, int windowY, int windowW, int windowH, SHP_WindowFlags windowFlags) {

    // Convert SHP_InitFlags to SDL_Init flags
    Uint32 sdlInitFlags = 0;
    if (initFlags & SHP_INIT_TIMER) sdlInitFlags |= SDL_INIT_TIMER;
    if (initFlags & SHP_INIT_AUDIO) sdlInitFlags |= SDL_INIT_AUDIO;
    if (initFlags & SHP_INIT_VIDEO) sdlInitFlags |= SDL_INIT_VIDEO;
    if (initFlags & SHP_INIT_JOYSTICK) sdlInitFlags |= SDL_INIT_JOYSTICK;
    if (initFlags & SHP_INIT_HAPTIC) sdlInitFlags |= SDL_INIT_HAPTIC;
    if (initFlags & SHP_INIT_GAMECONTROLLER) sdlInitFlags |= SDL_INIT_GAMECONTROLLER;
    if (initFlags & SHP_INIT_EVENTS) sdlInitFlags |= SDL_INIT_EVENTS;
    if (initFlags & SHP_INIT_NOPARACHUTE) sdlInitFlags |= SDL_INIT_NOPARACHUTE;
    if (initFlags & SHP_INIT_EVERYTHING) sdlInitFlags |= SDL_INIT_EVERYTHING;

    // SDL Initialization
    if (SDL_Init(sdlInitFlags) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // IMG Initialization (if needed)
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Convert SHP_WindowFlags to SDL_WindowFlags
    Uint32 sdlWindowFlags = 0;
    if (windowFlags & SHP_WINDOW_FULLSCREEN) sdlWindowFlags |= SDL_WINDOW_FULLSCREEN;
    if (windowFlags & SHP_WINDOW_OPENGL) sdlWindowFlags |= SDL_WINDOW_OPENGL;
    if (windowFlags & SHP_WINDOW_SHOWN) sdlWindowFlags |= SDL_WINDOW_SHOWN;
    if (windowFlags & SHP_WINDOW_HIDDEN) sdlWindowFlags |= SDL_WINDOW_HIDDEN;
    if (windowFlags & SHP_WINDOW_BORDERLESS) sdlWindowFlags |= SDL_WINDOW_BORDERLESS;
    if (windowFlags & SHP_WINDOW_RESIZABLE) sdlWindowFlags |= SDL_WINDOW_RESIZABLE;
    if (windowFlags & SHP_WINDOW_MINIMIZED) sdlWindowFlags |= SDL_WINDOW_MINIMIZED;
    if (windowFlags & SHP_WINDOW_MAXIMIZED) sdlWindowFlags |= SDL_WINDOW_MAXIMIZED;
    if (windowFlags & SHP_WINDOW_INPUT_GRABBED) sdlWindowFlags |= SDL_WINDOW_INPUT_GRABBED;
    if (windowFlags & SHP_WINDOW_INPUT_FOCUS) sdlWindowFlags |= SDL_WINDOW_INPUT_FOCUS;
    if (windowFlags & SHP_WINDOW_MOUSE_FOCUS) sdlWindowFlags |= SDL_WINDOW_MOUSE_FOCUS;
    if (windowFlags & SHP_WINDOW_FULLSCREEN_DESKTOP) sdlWindowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (windowFlags & SHP_WINDOW_FOREIGN) sdlWindowFlags |= SDL_WINDOW_FOREIGN;
    if (windowFlags & SHP_WINDOW_ALLOW_HIGHDPI) sdlWindowFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
    if (windowFlags & SHP_WINDOW_MOUSE_CAPTURE) sdlWindowFlags |= SDL_WINDOW_MOUSE_CAPTURE;
    if (windowFlags & SHP_WINDOW_ALWAYS_ON_TOP) sdlWindowFlags |= SDL_WINDOW_ALWAYS_ON_TOP;
    if (windowFlags & SHP_WINDOW_SKIP_TASKBAR) sdlWindowFlags |= SDL_WINDOW_SKIP_TASKBAR;
    if (windowFlags & SHP_WINDOW_UTILITY) sdlWindowFlags |= SDL_WINDOW_UTILITY;
    if (windowFlags & SHP_WINDOW_TOOLTIP) sdlWindowFlags |= SDL_WINDOW_TOOLTIP;
    if (windowFlags & SHP_WINDOW_POPUP_MENU) sdlWindowFlags |= SDL_WINDOW_POPUP_MENU;
    if (windowFlags & SHP_WINDOW_VULKAN) sdlWindowFlags |= SDL_WINDOW_VULKAN;
    if (windowFlags & SHP_WINDOW_METAL) sdlWindowFlags |= SDL_WINDOW_METAL;

    // Create Window
    _sdl_window = SDL_CreateWindow(
        windowTitle.c_str(),
        windowX,
        windowY,
        windowW,
        windowH,
        sdlWindowFlags
    );

    if (!_sdl_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Create Renderer
    _renderer = SDL_CreateRenderer(_sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(_sdl_window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Store window size
    _window_w = windowW;
    _window_h = windowH;

    return true;
}

void HardwareInterface::shutdown() {
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_sdl_window) {
        SDL_DestroyWindow(_sdl_window);
        _sdl_window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}

bool HardwareInterface::PopEvent(EventData& eventData) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        // Map SDL event type to SHP_InputType
        auto inputTypeIt = input_type_from_sdl_event.find(event.type);
        if (inputTypeIt != input_type_from_sdl_event.end()) {
            eventData.type = inputTypeIt->second;

            // For key events, map SDL_Keycode to SHP_Keycode
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                SDL_Keycode sdlKeycode = event.key.keysym.sym;
                auto keycodeIt = keycode_from_sdl_keycode.find(sdlKeycode);
                if (keycodeIt != keycode_from_sdl_keycode.end()) {
                    eventData.keycode = keycodeIt->second;
                }
                else {
                    eventData.keycode = SHP_K_UNKNOWN;
                }
            }
            else {
                eventData.keycode = SHP_K_UNKNOWN;
            }
            return true;
        }
        else {
            // Event type not mapped; you may choose to handle this differently
            eventData.type = SHP_FIRSTEVENT;
            eventData.keycode = SHP_K_UNKNOWN;
            return true; // or false, depending on whether you want to ignore unmapped events
        }
    }
    return false; // No event to process
}

void HardwareInterface::initInputMappings() {
    // Map SDL_EventType to SHP_InputType
    input_type_from_sdl_event = {
        {SDL_QUIT, SHP_QUIT},
        {SDL_APP_TERMINATING, SHP_APP_TERMINATING},
        {SDL_APP_LOWMEMORY, SHP_APP_LOWMEMORY},
        {SDL_APP_WILLENTERBACKGROUND, SHP_APP_WILLENTERBACKGROUND},
        {SDL_APP_DIDENTERBACKGROUND, SHP_APP_DIDENTERBACKGROUND},
        {SDL_APP_WILLENTERFOREGROUND, SHP_APP_WILLENTERFOREGROUND},
        {SDL_APP_DIDENTERFOREGROUND, SHP_APP_DIDENTERFOREGROUND},
        {SDL_LOCALECHANGED, SHP_LOCALECHANGED},

        {SDL_DISPLAYEVENT, SHP_DISPLAYEVENT},

        {SDL_WINDOWEVENT, SHP_WINDOWEVENT},
        {SDL_SYSWMEVENT, SHP_SYSWMEVENT},

        {SDL_KEYDOWN, SHP_KEYDOWN},
        {SDL_KEYUP, SHP_KEYUP},
        {SDL_TEXTEDITING, SHP_TEXTEDITING},
        {SDL_TEXTINPUT, SHP_TEXTINPUT},
        {SDL_KEYMAPCHANGED, SHP_KEYMAPCHANGED},
        {SDL_TEXTEDITING_EXT, SHP_TEXTEDITING_EXT},

        {SDL_MOUSEMOTION, SHP_MOUSEMOTION},
        {SDL_MOUSEBUTTONDOWN, SHP_MOUSEBUTTONDOWN},
        {SDL_MOUSEBUTTONUP, SHP_MOUSEBUTTONUP},
        {SDL_MOUSEWHEEL, SHP_MOUSEWHEEL},

        {SDL_JOYAXISMOTION, SHP_JOYAXISMOTION},
        {SDL_JOYBALLMOTION, SHP_JOYBALLMOTION},
        {SDL_JOYHATMOTION, SHP_JOYHATMOTION},
        {SDL_JOYBUTTONDOWN, SHP_JOYBUTTONDOWN},
        {SDL_JOYBUTTONUP, SHP_JOYBUTTONUP},
        {SDL_JOYDEVICEADDED, SHP_JOYDEVICEADDED},
        {SDL_JOYDEVICEREMOVED, SHP_JOYDEVICEREMOVED},
        {SDL_JOYBATTERYUPDATED, SHP_JOYBATTERYUPDATED},

        {SDL_CONTROLLERAXISMOTION, SHP_CONTROLLERAXISMOTION},
        {SDL_CONTROLLERBUTTONDOWN, SHP_CONTROLLERBUTTONDOWN},
        {SDL_CONTROLLERBUTTONUP, SHP_CONTROLLERBUTTONUP},
        {SDL_CONTROLLERDEVICEADDED, SHP_CONTROLLERDEVICEADDED},
        {SDL_CONTROLLERDEVICEREMOVED, SHP_CONTROLLERDEVICEREMOVED},
        {SDL_CONTROLLERDEVICEREMAPPED, SHP_CONTROLLERDEVICEREMAPPED},
        {SDL_CONTROLLERTOUCHPADDOWN, SHP_CONTROLLERTOUCHPADDOWN},
        {SDL_CONTROLLERTOUCHPADMOTION, SHP_CONTROLLERTOUCHPADMOTION},
        {SDL_CONTROLLERTOUCHPADUP, SHP_CONTROLLERTOUCHPADUP},
        {SDL_CONTROLLERSENSORUPDATE, SHP_CONTROLLERSENSORUPDATE},
        {SDL_CONTROLLERSTEAMHANDLEUPDATED, SHP_CONTROLLERSTEAMHANDLEUPDATED},

        {SDL_FINGERDOWN, SHP_FINGERDOWN},
        {SDL_FINGERUP, SHP_FINGERUP},
        {SDL_FINGERMOTION, SHP_FINGERMOTION},

        {SDL_DOLLARGESTURE, SHP_DOLLARGESTURE},
        {SDL_DOLLARRECORD, SHP_DOLLARRECORD},
        {SDL_MULTIGESTURE, SHP_MULTIGESTURE},

        {SDL_CLIPBOARDUPDATE, SHP_CLIPBOARDUPDATE},

        {SDL_DROPFILE, SHP_DROPFILE},
        {SDL_DROPTEXT, SHP_DROPTEXT},
        {SDL_DROPBEGIN, SHP_DROPBEGIN},
        {SDL_DROPCOMPLETE, SHP_DROPCOMPLETE},

        {SDL_AUDIODEVICEADDED, SHP_AUDIODEVICEADDED},
        {SDL_AUDIODEVICEREMOVED, SHP_AUDIODEVICEREMOVED},

        {SDL_SENSORUPDATE, SHP_SENSORUPDATE},

        {SDL_RENDER_TARGETS_RESET, SHP_RENDER_TARGETS_RESET},
        {SDL_RENDER_DEVICE_RESET, SHP_RENDER_DEVICE_RESET},

        {SDL_POLLSENTINEL, SHP_POLLSENTINEL},

        {SDL_USEREVENT, SHP_USEREVENT},
    };

    // Map SDL_Keycode to SHP_Keycode
    keycode_from_sdl_keycode = {
        MAP_KEYCODE(UNKNOWN, UNKNOWN),

        // Letters
        MAP_KEYCODE(a, A),
        MAP_KEYCODE(b, B),
        MAP_KEYCODE(c, C),
        MAP_KEYCODE(d, D),
        MAP_KEYCODE(e, E),
        MAP_KEYCODE(f, F),
        MAP_KEYCODE(g, G),
        MAP_KEYCODE(h, H),
        MAP_KEYCODE(i, I),
        MAP_KEYCODE(j, J),
        MAP_KEYCODE(k, K),
        MAP_KEYCODE(l, L),
        MAP_KEYCODE(m, M),
        MAP_KEYCODE(n, N),
        MAP_KEYCODE(o, O),
        MAP_KEYCODE(p, P),
        MAP_KEYCODE(q, Q),
        MAP_KEYCODE(r, R),
        MAP_KEYCODE(s, S),
        MAP_KEYCODE(t, T),
        MAP_KEYCODE(u, U),
        MAP_KEYCODE(v, V),
        MAP_KEYCODE(w, W),
        MAP_KEYCODE(x, X),
        MAP_KEYCODE(y, Y),
        MAP_KEYCODE(z, Z),

        // Numbers
        MAP_KEYCODE(0, 0),
        MAP_KEYCODE(1, 1),
        MAP_KEYCODE(2, 2),
        MAP_KEYCODE(3, 3),
        MAP_KEYCODE(4, 4),
        MAP_KEYCODE(5, 5),
        MAP_KEYCODE(6, 6),
        MAP_KEYCODE(7, 7),
        MAP_KEYCODE(8, 8),
        MAP_KEYCODE(9, 9),

        // Function keys
        MAP_KEYCODE(F1, F1),
        MAP_KEYCODE(F2, F2),
        MAP_KEYCODE(F3, F3),
        MAP_KEYCODE(F4, F4),
        MAP_KEYCODE(F5, F5),
        MAP_KEYCODE(F6, F6),
        MAP_KEYCODE(F7, F7),
        MAP_KEYCODE(F8, F8),
        MAP_KEYCODE(F9, F9),
        MAP_KEYCODE(F10, F10),
        MAP_KEYCODE(F11, F11),
        MAP_KEYCODE(F12, F12),

        // Control keys
        MAP_KEYCODE(RETURN, RETURN),
        MAP_KEYCODE(ESCAPE, ESCAPE),
        MAP_KEYCODE(BACKSPACE, BACKSPACE),
        MAP_KEYCODE(TAB, TAB),
        MAP_KEYCODE(SPACE, SPACE),
        MAP_KEYCODE(DELETE, DELETE),

        // Arrow keys
        MAP_KEYCODE(RIGHT, RIGHT),
        MAP_KEYCODE(LEFT, LEFT),
        MAP_KEYCODE(DOWN, DOWN),
        MAP_KEYCODE(UP, UP),

        // Modifier keys
        MAP_KEYCODE(LSHIFT, LSHIFT),
        MAP_KEYCODE(RSHIFT, RSHIFT),
        MAP_KEYCODE(LCTRL, LCTRL),
        MAP_KEYCODE(RCTRL, RCTRL),
        MAP_KEYCODE(LALT, LALT),
        MAP_KEYCODE(RALT, RALT),

        // Punctuation and symbols
        MAP_KEYCODE(EXCLAIM, EXCLAIM),
        MAP_KEYCODE(QUOTEDBL, QUOTEDBL),
        MAP_KEYCODE(HASH, HASH),
        MAP_KEYCODE(PERCENT, PERCENT),
        MAP_KEYCODE(DOLLAR, DOLLAR),
        MAP_KEYCODE(AMPERSAND, AMPERSAND),
        MAP_KEYCODE(QUOTE, QUOTE),
        MAP_KEYCODE(LEFTPAREN, LEFTPAREN),
        MAP_KEYCODE(RIGHTPAREN, RIGHTPAREN),
        MAP_KEYCODE(ASTERISK, ASTERISK),
        MAP_KEYCODE(PLUS, PLUS),
        MAP_KEYCODE(COMMA, COMMA),
        MAP_KEYCODE(MINUS, MINUS),
        MAP_KEYCODE(PERIOD, PERIOD),
        MAP_KEYCODE(SLASH, SLASH),
        MAP_KEYCODE(COLON, COLON),
        MAP_KEYCODE(SEMICOLON, SEMICOLON),
        MAP_KEYCODE(LESS, LESS),
        MAP_KEYCODE(EQUALS, EQUALS),
        MAP_KEYCODE(GREATER, GREATER),
        MAP_KEYCODE(QUESTION, QUESTION),
        MAP_KEYCODE(AT, AT),
        MAP_KEYCODE(LEFTBRACKET, LEFTBRACKET),
        MAP_KEYCODE(BACKSLASH, BACKSLASH),
        MAP_KEYCODE(RIGHTBRACKET, RIGHTBRACKET),
        MAP_KEYCODE(CARET, CARET),
        MAP_KEYCODE(UNDERSCORE, UNDERSCORE),
        MAP_KEYCODE(BACKQUOTE, BACKQUOTE),

        // Keypad keys
        MAP_KEYCODE(KP_0, KP_0),
        MAP_KEYCODE(KP_1, KP_1),
        MAP_KEYCODE(KP_2, KP_2),
        MAP_KEYCODE(KP_3, KP_3),
        MAP_KEYCODE(KP_4, KP_4),
        MAP_KEYCODE(KP_5, KP_5),
        MAP_KEYCODE(KP_6, KP_6),
        MAP_KEYCODE(KP_7, KP_7),
        MAP_KEYCODE(KP_8, KP_8),
        MAP_KEYCODE(KP_9, KP_9),
        MAP_KEYCODE(KP_PERIOD, KP_PERIOD),
        MAP_KEYCODE(KP_DIVIDE, KP_DIVIDE),
        MAP_KEYCODE(KP_MULTIPLY, KP_MULTIPLY),
        MAP_KEYCODE(KP_MINUS, KP_MINUS),
        MAP_KEYCODE(KP_PLUS, KP_PLUS),
        MAP_KEYCODE(KP_ENTER, KP_ENTER),
        MAP_KEYCODE(KP_EQUALS, KP_EQUALS),

        // Other keys
        MAP_KEYCODE(INSERT, INSERT),
        MAP_KEYCODE(HOME, HOME),
        MAP_KEYCODE(END, END),
        MAP_KEYCODE(PAGEUP, PAGEUP),
        MAP_KEYCODE(PAGEDOWN, PAGEDOWN),

        // Media keys
        MAP_KEYCODE(MUTE, MUTE),
        MAP_KEYCODE(VOLUMEUP, VOLUMEUP),
        MAP_KEYCODE(VOLUMEDOWN, VOLUMEDOWN),

        // Add other keycode mappings as needed
    };
}
