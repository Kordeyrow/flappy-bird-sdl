#include "input_manager.h"

InputManager::InputManager() {
}

InputManager::~InputManager() {
}

void InputManager::update(const std::vector<EventData>& events) {
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
        // Handle other input events (mouse, joystick) as needed
    }
}

bool InputManager::is_key_down(SHP_Keycode keycode) const {
    auto it = keys_down.find(keycode);
    return it != keys_down.end() && it->second;
}

bool InputManager::is_key_just_pressed(SHP_Keycode keycode) const {
    auto it = keys_just_pressed.find(keycode);
    return it != keys_just_pressed.end() && it->second;
}

bool InputManager::is_key_just_released(SHP_Keycode keycode) const {
    auto it = keys_just_released.find(keycode);
    return it != keys_just_released.end() && it->second;
}

