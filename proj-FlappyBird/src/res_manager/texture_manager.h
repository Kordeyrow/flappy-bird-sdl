#pragma once
#include <map>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <wing.h>

using namespace WING_API;

enum TEXTURE_KEY {
	TEXTURE_FLAPPY_BIRD_UP_WING,
	PIPE
};

class TextureManager {
private:
	// config
	const std::string TEXTURE_PATH = "assets/sprites/";
	std::map<TEXTURE_KEY, std::string> path_from_texture_key{
		{ static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING), TEXTURE_PATH + "yellowbird-upflap.png" },
		{ static_cast<TEXTURE_KEY>(PIPE), TEXTURE_PATH + "pipe-green.png" }
	};
	std::array<TEXTURE_KEY, 2> textures_to_load_on_init{
		static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING),
		static_cast<TEXTURE_KEY>(PIPE)
	};
	// dependency
	//std::shared_ptr<AssetManager> asset_manager;
	// runtime
	std::map<TEXTURE_KEY, AssetID> loaded_textures{};

public:
	//TextureManager(std::shared_ptr<AssetManager> asset_manager)
	//	: asset_manager{ asset_manager } {
	//}
	TextureManager() {
	}

	void init() {
		load_init_textures();
	}

	AssetID get_texture_id(TEXTURE_KEY key)	{
		ensure_is_loaded(key);
		return loaded_textures[key];
	}

	TextureData get_texture_data(TEXTURE_KEY key) {
		ensure_is_loaded(key);
		return AssetManager::get_texture_data(path_from_texture_key[key]);
	}

	void ensure_is_loaded(TEXTURE_KEY key) {
		load_texture(key);
	}

private:
	void load_init_textures() {
		for (TEXTURE_KEY key : textures_to_load_on_init) {
			load_texture(key);
		}
	}

	AssetID load_texture(TEXTURE_KEY key) {
		auto id = AssetManager::load_texture(path_from_texture_key[key]);
		loaded_textures[key] = id;
		return id;
	}


};