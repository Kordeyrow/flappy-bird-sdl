#pragma once
#include <dll/export_def.h> // dll
#include <cinttypes> // cpp
#include <memory>
#include <containers/containers.h> // internal

class BIRDENGINE_API Registry {
private:
	std::vector<GameObject*> all_gameobjects_list;
	std::map<GameObjectID, GameObject*> gameobject_from_id;
	std::map<GameObjectID, GameObject*> gameobject_onscene_list;

	std::vector<Scene*> all_scenes_list;
	std::map<SceneID, Scene*> scene_from_id;
	SceneID starting_scene_id;
	/*SceneID current_editing_scene_id;
	std::vector<SceneID> build_scene_list;*/

	SceneID create_new_scene();
	void set_starting_scene(SceneID id);
	GameObject* create_new_gameobject();
	void add_gameobject_to_scene(GameObject* go_id, SceneID scene_id);

public:
	Registry(const Registry&) = delete;
	Registry& operator=(const Registry&) = delete;
	static std::shared_ptr<Registry> instance();

// TODO: internal only
public:
	/*Scene* init_start_scene() {
		scene_from_id[starting_scene_id]->init();
		return scene_from_id[starting_scene_id];
	}*/

private:
	Registry();
	~Registry();

};

