#pragma once
#include <dll/export_def.h> // dll
#include <memory>
//#include <containers/containers.h> // internal

//struct Registry;

struct BIRDENGINE_API RegistryAPI {
	RegistryAPI(const RegistryAPI&) = delete;
	RegistryAPI& operator=(const RegistryAPI&) = delete;
	static std::shared_ptr<RegistryAPI> instance();

	void test();
	void oi();
private:
	RegistryAPI();
	~RegistryAPI();


	//SceneID create_new_scene();


	/*void set_starting_scene(SceneID id);
	GameObject* create_new_gameobject();
	void add_gameobject_to_scene(GameObject* go_id, SceneID scene_id);*/
};