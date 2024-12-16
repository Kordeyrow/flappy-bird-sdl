#include "registry.h"
#include <registry.h>
#include <iostream>

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

	void Registry::test() {
		std::cout << "aaaaaaaaaaaa" << std::endl;
	}

	SceneID Registry::create_new_scene() {
		Scene* new_scene = new Scene();
		instance()->all_scenes_list.push_back(new_scene);
		instance()->scene_from_id.insert({ new_scene->id(), new_scene });
		return new_scene->id();
	}

	void Registry::set_starting_scene(SceneID id) {
		if (instance()->scene_from_id.find(id) != instance()->scene_from_id.end()) {
			instance()->starting_scene_id = id;
		}
	}

	GameObject* Registry::create_new_gameobject() {
		GameObject* new_gameobject = new GameObject();
		instance()->all_gameobjects_list.push_back(new_gameobject);
		instance()->gameobject_from_id.insert({ new_gameobject->id(), new_gameobject });
		return new_gameobject;
	}

	void Registry::add_gameobject_to_scene(GameObject* go_id, SceneID scene_id) {
		if (instance()->scene_from_id.find(scene_id) != instance()->scene_from_id.end()) {
			instance()->scene_from_id[scene_id]->gameobject_list.push_back(go_id);
		}
	}
}