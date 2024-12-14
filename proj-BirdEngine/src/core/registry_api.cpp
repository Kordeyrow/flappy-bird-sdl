#include <registry.h>
#include "core/registry.h"

namespace BIRDENGINE_API {
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

	SceneID Registry::create_new_scene()
	{
		return Registry::instance()->create_new_scene();
	}

	void Registry::set_starting_scene(SceneID id)
	{
		Registry::instance()->set_starting_scene(id);
	}

	GameObject* Registry::create_new_gameobject()
	{
		return Registry::instance()->create_new_gameobject();
	}

	void Registry::add_gameobject_to_scene(GameObject* go_id, SceneID scene_id)
	{
		Registry::instance()->add_gameobject_to_scene(go_id, scene_id);
	}
}
