#pragma once
#include "declspec.h"
#include "containers.h"
#include "wing_events.h"

namespace WING_API {
	struct DECLSPEC InputManager {
        static bool is_key_down(WING_Keycode keycode);
        static bool is_key_just_pressed(WING_Keycode keycode);
        static bool is_key_just_released(WING_Keycode keycode);
	};
}