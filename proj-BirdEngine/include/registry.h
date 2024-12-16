#pragma once
#include "declspec.h"
#include "containers.h"
#include <memory>

namespace WING_API {
	struct DECLSPEC Registry {
		static SceneID create_new_scene();
		static void set_starting_scene(SceneID id);
		static GameObject* create_new_gameobject();
		static void add_gameobject_to_scene(GameObject* go_id, SceneID scene_id);
	};
}