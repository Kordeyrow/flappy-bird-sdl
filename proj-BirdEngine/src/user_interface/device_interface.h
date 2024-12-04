#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <user_interface/gui_manager.h>
#include <containers/containers.h>
#include "SHP_events.h"
#include "gui_manager.h"
#include "window.h"
#include "renderer.h"
#include <iostream>
#include <string>

class DeviceInterface {
private:
    // dependency
    std::shared_ptr<IOManager> io_manager;
    // owns
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<GUIManager> gui_manager;
    // runtime
    bool initialized = false;
    std::vector<EventData> events;
    std::map<uint32_t, SHP_InputType> input_type_from_sdl_event;
    std::map<uint32_t, SHP_Keycode> keycode_from_sdl_keycode;

public:
    DeviceInterface(std::shared_ptr<IOManager> _io_manager) 
        : io_manager{_io_manager } {
        init_input_mappings();
    }

    ~DeviceInterface() {
        IMG_Quit();
        SDL_Quit();
    }

    bool init() {
        if (initialized) {
            io_manager->print_line_warning("DeviceManager already initilized");
        };
        // SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            io_manager->print_line_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
            SDL_Quit();
            return false;
        }
        // SDL_image
        if (IMG_Init(IMG_INIT_PNG) < 0) {
            io_manager->print_line_error("Failed to initialize SDL_image: " + std::string(IMG_GetError()));
            IMG_Quit();
            SDL_Quit();
            return false;
        }

        window = std::make_shared<Window>();
        if (window->init()) {
            return false;
        }

        renderer = std::make_shared<Renderer>();
        if (renderer->init()) {
            return false;
        }

        gui_manager = std::make_shared<GUIManager>();
        if (gui_manager->init(GUIManagerInitParams{})) {
            return false;
        }

        initialized = true;
        return true;
    }

    void set_window_rect(WindowRect rect) {
        SDL_SetWindowSize(window, rect.size.x, rect.size.y);
        SDL_SetWindowPosition(window, rect.position.x, rect.position.y);
    }

    const std::vector<EventData>& get_events() { return events; }

    void update_events() {
        events.clear();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            add_event(&event);
        }
    }
private:
    void init_input_mappings() {
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

#define MAP_KEYCODE(sdlk, shpk) { SDLK_##sdlk, SHP_K_##shpk }
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

	void init_window(WindowInitData data) {
		set_window_rect(data.rect);
		SDL_SetWindowTitle(window, data.title.c_str());
	}

    void add_event(SDL_Event* sdl_event) {
        auto input_type_it = input_type_from_sdl_event.find(sdl_event->type);
        if (input_type_it != input_type_from_sdl_event.end()) {
            EventData event_data;
            event_data.type = input_type_it->second;

            if (sdl_event->type == SDL_KEYDOWN || sdl_event->type == SDL_KEYUP) {
                SDL_Keycode sdl_keycode = sdl_event->key.keysym.sym;
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
    ImFont* add_font(std::string path, float size_pixels) {
        return ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), size_pixels);
    }

    void set_dark_colorstyle() {
        ImGui::StyleColorsDark();
    }
};

