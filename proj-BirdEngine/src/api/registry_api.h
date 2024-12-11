#pragma once
#include <dll/export_def.h> // dll
#include <cinttypes> // cpp
#include <memory>
#include <containers/containers.h> // internal

using SceneID = uint32_t;

struct BIRDENGINE_API Registry {
	//static std::shared_ptr<Registry> instance();
	virtual SceneID create_new_scene() = 0;
	virtual void set_starting_scene(SceneID id) = 0;
	virtual GameObject* create_new_gameobject() = 0;
	virtual void add_gameobject_to_scene(GameObject* go_id, SceneID scene_id) = 0;
};