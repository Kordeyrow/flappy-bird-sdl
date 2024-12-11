#pragma once

#include <dll/export_def.h>
#include <SDL.h>
#include "SHP_events.h"
#include "input_manager.h"

class BIRDENGINE_API UserInterface {
public:
    UserInterface();
    ~UserInterface();

    bool init(const InitParams& params);
    void shutdown();
    void update_events();

    // Input handling methods
    bool is_key_down(SHP_Keycode keycode) const;
    bool is_key_just_pressed(SHP_Keycode keycode) const;
    bool is_key_just_released(SHP_Keycode keycode) const;

private:
    // Internal methods
    void init_input_mappings();
    void add_event(void* sdl_event); // Using void* to avoid exposing SDL_Event

    // Map SDL event types to SHP_InputType
    std::map<uint32_t, SHP_InputType> input_type_from_sdl_event;
    // Map SDL_Keycode to SHP_Keycode
    std::map<uint32_t, SHP_Keycode> keycode_from_sdl_keycode;

    // Events vector
    std::vector<EventData> events;

    // InputManager instance
    InputManager input_manager;

    // Internal SDL pointers (opaque to the user)
    SDL_Window* _window;   // SDL_Window*
    void* _renderer; // SDL_Renderer*

    bool initialized = false;
};

