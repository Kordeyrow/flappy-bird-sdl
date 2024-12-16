#pragma once
#include "containers.h" 
#include <memory>
#include <iostream>
#include <cinttypes> 

namespace WING {
	class Registry {
	private:
		std::vector<GameObject*> all_gameobjects_list;
		std::map<GameObjectID, GameObject*> id_gameobject_map;
		std::map<GameObjectID, GameObject*> gameobject_onscene_list;

		std::vector<Scene*> all_scenes_list;
		std::map<SceneID, Scene*> id_scene_map;
		SceneID starting_scene_id;
		/*SceneID current_editing_scene_id;
		std::vector<SceneID> build_scene_list;*/

	public:

		SceneID create_new_scene();
		void set_starting_scene(SceneID id);
		GameObject* create_new_gameobject();
		void add_gameobject_to_scene(GameObject* go_id, SceneID scene_id);

		// TODO: internal only
	//public:
		/*Scene* init_start_scene() {
			scene_from_id[starting_scene_id]->init();
			return scene_from_id[starting_scene_id];
		}*/
	};
}