#pragma once
#include "declspec.h"
#include "containers.h"
#include <memory>

namespace BIRDENGINE_API {
	struct DECLSPEC Registry {
		Registry(const Registry&) = delete;
		Registry& operator=(const Registry&) = delete;
		static std::shared_ptr<Registry> instance();

		SceneID create_new_scene();
		void set_starting_scene(SceneID id);
		GameObject* create_new_gameobject();
		void add_gameobject_to_scene(GameObject* go_id, SceneID scene_id);

	private:
		Registry();
		~Registry();
	};

}