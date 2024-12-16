#pragma once
#include <SDL.h>
#include "wing_events.h"
#include "containers.h"
#include <vector>
#include <unordered_map>

namespace WING {
    class InputManager {
    private:
        // config
        std::map<uint32_t, WING_InputType> input_type_from_sdl_event;
        std::map<uint32_t, WING_Keycode> keycode_from_sdl_keycode;
        // runtime
        std::vector<EventData> events;
        std::unordered_map<WING_Keycode, bool> keys_down;
        std::unordered_map<WING_Keycode, bool> keys_just_pressed;
        std::unordered_map<WING_Keycode, bool> keys_just_released;

    public:
        InputManager();
        ~InputManager();

        bool is_key_down(WING_Keycode keycode) const;
        bool is_key_just_pressed(WING_Keycode keycode) const;
        bool is_key_just_released(WING_Keycode keycode) const;

        ProgramState update(float elapsed_time_seconds);

        // getters
        const std::vector<EventData>& get_events();

    private:
        void update_events();

        void add_event(SDL_Event* sdl_event);

        void init_input_mappings();
    };
}

