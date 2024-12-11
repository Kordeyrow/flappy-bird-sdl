#pragma once

#include "SHP_events.h"
#include <unordered_map>
#include <vector>

class InputManager {
public:
    InputManager();
    ~InputManager();

    // Input handling methods
    bool is_key_down(SHP_Keycode keycode) const;
    bool is_key_just_pressed(SHP_Keycode keycode) const;
    bool is_key_just_released(SHP_Keycode keycode) const;

    // Internal method to update the input states (only accessible by UserInterface)
    void update(const std::vector<EventData>& events);

private:
    // Store key states
    std::unordered_map<SHP_Keycode, bool> keys_down;
    std::unordered_map<SHP_Keycode, bool> keys_just_pressed;
    std::unordered_map<SHP_Keycode, bool> keys_just_released;
};

