#pragma once
#include <dll/export_def.h> // dll
#include <cinttypes> // cpp
#include <memory>
#include <containers/containers.h> // internal
#include <api/registry_api.h>

class BIRDENGINE_API RegistryInternal : public Registry {
private:
	std::vector<GameObject*> all_gameobjects_list;
	std::map<GameObjectID, GameObject*> gameobject_from_id;
	std::map<GameObjectID, GameObject*> gameobject_onscene_list;

	std::vector<Scene*> all_scenes_list;
	std::map<SceneID, Scene*> scene_from_id;
	SceneID starting_scene_id;
	/*SceneID current_editing_scene_id;
	std::vector<SceneID> build_scene_list;*/

public:
	RegistryInternal(const RegistryInternal&) = delete;
	RegistryInternal& operator=(const RegistryInternal&) = delete;
	static std::shared_ptr<RegistryInternal> instance();

// TODO: internal only
public:
	Scene* init_start_scene() {
		scene_from_id[starting_scene_id]->init();
		return scene_from_id[starting_scene_id];
	}

private:
	RegistryInternal();
	~RegistryInternal();

};

