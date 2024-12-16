#include "registry.h"
#include <registry.h>
#include <iostream>

namespace WING_API {
	SceneID Registry::create_new_scene() {
		return WING::Registry::instance()->create_new_scene();
	}

	void Registry::set_starting_scene(SceneID id) {
		WING::Registry::instance()->set_starting_scene(id);
	}

	GameObject* Registry::create_new_gameobject() {
		return WING::Registry::instance()->create_new_gameobject();
	}

	void Registry::add_gameobject_to_scene(GameObject* go_id, SceneID scene_id) {
		WING::Registry::instance()->add_gameobject_to_scene(go_id, scene_id);
	}
}

namespace WING {
	Registry::Registry() {};
	Registry::~Registry() {};

	std::shared_ptr<Registry> Registry::instance() {
		static std::shared_ptr<Registry> instance(
			new Registry(),
			[](Registry* reg) {
				delete reg; // Custom deleter to allow destruction of the singleton
			}
		);
		return instance;
	}

	SceneID Registry::create_new_scene() {
		Scene* new_scene = new Scene();
		instance()->all_scenes_list.push_back(new_scene);
		instance()->id_scene_map.insert({ new_scene->id(), new_scene });
		return new_scene->id();
	}

	void Registry::set_starting_scene(SceneID id) {
		if (instance()->id_scene_map.find(id) != instance()->id_scene_map.end()) {
			instance()->starting_scene_id = id;
		}
	}

	GameObject* Registry::create_new_gameobject() {
		GameObject* new_gameobject = new GameObject();
		instance()->all_gameobjects_list.push_back(new_gameobject);
		instance()->id_gameobject_map.insert({ new_gameobject->id(), new_gameobject });
		return new_gameobject;
	}

	void Registry::add_gameobject_to_scene(GameObject* go_id, SceneID scene_id) {
		if (instance()->id_scene_map.find(scene_id) != instance()->id_scene_map.end()) {
			instance()->id_scene_map[scene_id]->gameobject_list.push_back(go_id);
		}
	}
}