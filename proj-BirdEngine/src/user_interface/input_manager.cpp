#include "input_manager.h"

InputManager::InputManager() {
}

InputManager::~InputManager() {
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

