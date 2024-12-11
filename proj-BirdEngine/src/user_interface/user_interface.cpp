#include "user_interface.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

// Macros for mapping keycodes
#define MAP_KEYCODE(sdlk, shpk) { SDLK_##sdlk, SHP_K_##shpk }

UserInterface::UserInterface()
    : _window(nullptr), _renderer(nullptr) 
    {
    init_input_mappings();
}

UserInterface::~UserInterface() {
    shutdown();
}

bool UserInterface::init(const InitParams& params) {
    // Convert SHP_InitFlags to SDL_Init flags
    Uint32 sdl_init_flags = 0;
    if (params.init_flags & SHP_INIT_TIMER) sdl_init_flags |= SDL_INIT_TIMER;
    if (params.init_flags & SHP_INIT_AUDIO) sdl_init_flags |= SDL_INIT_AUDIO;
    if (params.init_flags & SHP_INIT_VIDEO) sdl_init_flags |= SDL_INIT_VIDEO;
    // Add other init flags as needed

    // Initialize SDL
    if (SDL_Init(sdl_init_flags) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_image if needed
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Convert SHP_WindowFlags to SDL_WindowFlags
    Uint32 sdl_window_flags = 0;
    if (params.window_flags & SHP_WINDOW_SHOWN) sdl_window_flags |= SDL_WINDOW_SHOWN;
    if (params.window_flags & SHP_WINDOW_FULLSCREEN) sdl_window_flags |= SDL_WINDOW_FULLSCREEN;
    // Add other window flags as needed

    // Convert SHP_WindowPosition to SDL positions
    int window_x = SDL_WINDOWPOS_UNDEFINED;
    int window_y = SDL_WINDOWPOS_UNDEFINED;
    if (params.window_x == SHP_WINDOWPOS_CENTERED) window_x = SDL_WINDOWPOS_CENTERED;
    if (params.window_y == SHP_WINDOWPOS_CENTERED) window_y = SDL_WINDOWPOS_CENTERED;

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        params.window_title.c_str(),
        window_x,
        window_y,
        params.window_w,
        params.window_h,
        sdl_window_flags
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Store window and renderer as void* to avoid exposing SDL types
    _window = window;
    _renderer = renderer;

    initialized = true;

    return true;
}

void UserInterface::shutdown() {
    if ( ! initialized) return;
    if (_renderer) {
        SDL_DestroyRenderer(static_cast<SDL_Renderer*>(_renderer));
        _renderer = nullptr;
    }
    if (_window) {
        SDL_DestroyWindow(static_cast<SDL_Window*>(_window));
        _window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}

void UserInterface::update_events() {
    events.clear();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        add_event(&event);
    }
    input_manager.update(events);
}

void UserInterface::add_event(void* sdl_event) {
    SDL_Event* event = static_cast<SDL_Event*>(sdl_event);
    auto input_type_it = input_type_from_sdl_event.find(event->type);
    if (input_type_it != input_type_from_sdl_event.end()) {
        EventData event_data;
        event_data.type = input_type_it->second;

        if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
            SDL_Keycode sdl_keycode = event->key.keysym.sym;
            auto keycode_it = keycode_from_sdl_keycode.find(sdl_keycode);
            if (keycode_it != keycode_from_sdl_keycode.end()) {
                event_data.keycode = keycode_it->second;
            }
            else {
                event_data.keycode = SHP_K_UNKNOWN;
            }
        }
        else {
            event_data.keycode = SHP_K_UNKNOWN;
        }

        events.push_back(event_data);
    }
}

bool UserInterface::is_key_down(SHP_Keycode keycode) const {
    return input_manager.is_key_down(keycode);
}

bool UserInterface::is_key_just_pressed(SHP_Keycode keycode) const {
    return input_manager.is_key_just_pressed(keycode);
}

bool UserInterface::is_key_just_released(SHP_Keycode keycode) const {
    return input_manager.is_key_just_released(keycode);
}

void UserInterface::init_input_mappings() {
    // Initialize input_type_from_sdl_event
    input_type_from_sdl_event = {
        {SDL_QUIT, SHP_QUIT},
        {SDL_KEYDOWN, SHP_KEYDOWN},
        {SDL_KEYUP, SHP_KEYUP},
        {SDL_MOUSEMOTION, SHP_MOUSEMOTION},
        {SDL_MOUSEBUTTONDOWN, SHP_MOUSEBUTTONDOWN},
        {SDL_MOUSEBUTTONUP, SHP_MOUSEBUTTONUP},
        {SDL_MOUSEWHEEL, SHP_MOUSEWHEEL},
        {SDL_FINGERDOWN, SHP_FINGERDOWN},
        {SDL_FINGERUP, SHP_FINGERUP},
        {SDL_FINGERMOTION, SHP_FINGERMOTION},
        // Add other event mappings as needed
    };

    // Initialize keycode_from_sdl_keycode
    keycode_from_sdl_keycode = {
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

    };
}

