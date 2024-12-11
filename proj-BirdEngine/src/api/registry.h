#pragma once
#include <dll/export_def.h> // dll
#include <memory>
#include <containers/containers.h>

struct BIRDENGINE_API RegistryAPI {
	RegistryAPI(const RegistryAPI&) = delete;
	RegistryAPI& operator=(const RegistryAPI&) = delete;
	static std::shared_ptr<RegistryAPI> instance();


	SceneID create_new_scene();
	void set_starting_scene(SceneID id);
	GameObject* create_new_gameobject();
	void add_gameobject_to_scene(GameObject* go_id, SceneID scene_id);

private:
	RegistryAPI();
	~RegistryAPI();
};