#pragma once
#include <dll/export_def.h>
#include <SDL.h>
#include "SHP_events.h"
#include <unordered_map>
#include <vector>

typedef enum {
    RUNNING,
    QUIT
} ProgramState;

class BIRDENGINE_API InputManager {
private:
    // config
    std::map<uint32_t, SHP_InputType> input_type_from_sdl_event;
    std::map<uint32_t, SHP_Keycode> keycode_from_sdl_keycode;
    // runtime
    std::vector<EventData> events;
    std::unordered_map<SHP_Keycode, bool> keys_down;
    std::unordered_map<SHP_Keycode, bool> keys_just_pressed;
    std::unordered_map<SHP_Keycode, bool> keys_just_released;

public:
    InputManager();
    ~InputManager();

    bool is_key_down(SHP_Keycode keycode) const;
    bool is_key_just_pressed(SHP_Keycode keycode) const;
    bool is_key_just_released(SHP_Keycode keycode) const;

    ProgramState update(float elapsed_time_seconds);

    // getters
    const std::vector<EventData>& get_events();

private:
    void update_events();

    void add_event(SDL_Event* sdl_event);

    void init_input_mappings();
};

