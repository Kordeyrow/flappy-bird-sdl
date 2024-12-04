#pragma once
#include <dll/export_def.h>
#include "SHP_events.h"
#include <unordered_map>
#include <vector>

class BIRDENGINE_API InputManager {
public:
    InputManager();
    ~InputManager();

    bool is_key_down(SHP_Keycode keycode) const;
    bool is_key_just_pressed(SHP_Keycode keycode) const;
    bool is_key_just_released(SHP_Keycode keycode) const;

    void update(const std::vector<EventData>& events) {
        keys_just_pressed.clear();
        keys_just_released.clear();

        for (const auto& event : events) {
            if (event.type == SHP_KEYDOWN) {
                if (!keys_down[event.keycode]) {
                    keys_just_pressed[event.keycode] = true;
                }
                keys_down[event.keycode] = true;
            }
            else if (event.type == SHP_KEYUP) {
                if (keys_down[event.keycode]) {
                    keys_just_released[event.keycode] = true;
                }
                keys_down[event.keycode] = false;
            }
            // TODO: joystick
        }
    }

private:
    std::unordered_map<SHP_Keycode, bool> keys_down;
    std::unordered_map<SHP_Keycode, bool> keys_just_pressed;
    std::unordered_map<SHP_Keycode, bool> keys_just_released;
};

