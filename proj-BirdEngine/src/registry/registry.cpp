#include "registry.h"
#include <registry.h>
#include <engine_core.h>
#include <iostream>

namespace WING_API {
	SceneID Registry::create_new_scene() {
		return WING::EngineCore::instance()->registry()->create_new_scene();
	}

	void Registry::set_starting_scene(SceneID id) {
		WING::EngineCore::instance()->registry()->set_starting_scene(id);
	}

	GameObject* Registry::create_new_gameobject() {
		return WING::EngineCore::instance()->registry()->create_new_gameobject();
	}

	void Registry::add_gameobject_to_scene(GameObject* go_id, SceneID scene_id) {
		WING::EngineCore::instance()->registry()->add_gameobject_to_scene(go_id, scene_id);
	}
}

namespace WING {

	SceneID Registry::create_new_scene() {
		Scene* new_scene = new Scene();
		all_scenes_list.push_back(new_scene);
		id_scene_map.insert({ new_scene->id(), new_scene });
		return new_scene->id();
	}

	void Registry::set_starting_scene(SceneID id) {
		if (id_scene_map.find(id) != id_scene_map.end()) {
			starting_scene_id = id;
		}
	}

	GameObject* Registry::create_new_gameobject() {
		GameObject* new_gameobject = new GameObject();
		all_gameobjects_list.push_back(new_gameobject);
		id_gameobject_map.insert({ new_gameobject->id(), new_gameobject });
		return new_gameobject;
	}

	void Registry::add_gameobject_to_scene(GameObject* go_id, SceneID scene_id) {
		if (id_scene_map.find(scene_id) != id_scene_map.end()) {
			id_scene_map[scene_id]->gameobject_list.push_back(go_id);
		}
	}
}