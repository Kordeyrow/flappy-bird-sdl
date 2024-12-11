#include "api/registry.h"
#include "core/registry.h"
//#include <cinttypes>

// TODO: use same class name but different namespace
//namespace API {}

RegistryAPI::RegistryAPI() {};
RegistryAPI::~RegistryAPI() {};
std::shared_ptr<RegistryAPI> RegistryAPI::instance(){
	static std::shared_ptr<RegistryAPI> instance (
		new RegistryAPI(),
		[](RegistryAPI* reg) {
			delete reg; // Custom deleter to allow destruction of the singleton
		}
	);
	return instance;
}

SceneID RegistryAPI::create_new_scene()
{
	return Registry::instance()->create_new_scene();
}

void RegistryAPI::set_starting_scene(SceneID id)
{
	Registry::instance()->set_starting_scene(id);
}

GameObject* RegistryAPI::create_new_gameobject()
{
	return Registry::instance()->create_new_gameobject();
}

void RegistryAPI::add_gameobject_to_scene(GameObject* go_id, SceneID scene_id)
{
	Registry::instance()->add_gameobject_to_scene(go_id, scene_id);
}
