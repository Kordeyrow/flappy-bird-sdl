#include <registry.h>
#include "core/registry.h"

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

	void Registry::add_gameobject_to_scene(GameObject* go_id, SceneID scene_id) 	{
		WING::Registry::instance()->add_gameobject_to_scene(go_id, scene_id);
	}
}
