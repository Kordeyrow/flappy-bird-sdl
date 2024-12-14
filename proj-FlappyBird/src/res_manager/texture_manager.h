#pragma once
#include <map>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <WING.h>
using namespace BIRDENGINE_API;

enum TEXTURE_KEY {
	TEXTURE_FLAPPY_BIRD_UP_WING,
	PIPE
};

class TextureManager {
private:
	// config
	const std::string TEXTURE_PATH = "assets/sprites/";
	std::map<TEXTURE_KEY, std::string> path_from_texture_key{
		{ static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING), "yellowbird-upflap.png" },
		{ static_cast<TEXTURE_KEY>(PIPE), "pipe-green.png" }
	};
	std::array<TEXTURE_KEY, 2> textures_to_load_on_init{
		static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING),
		static_cast<TEXTURE_KEY>(PIPE)
	};
	// dependency
	std::shared_ptr<AssetManager> asset_manager;
	// runtime
	std::map<TEXTURE_KEY, AssetID> loaded_textures;

public:
	TextureManager(std::shared_ptr<AssetManager> asset_manager)
		: asset_manager{ asset_manager } {
	}

	void init() {
		load_init_textures();
	}

	AssetID get_texture_id(TEXTURE_KEY key)	{
		if (loaded_textures.find(key) != loaded_textures.end())
			return loaded_textures[key];
		return load_texture(key);
	}

private:
	void load_init_textures() {
		for (TEXTURE_KEY key : textures_to_load_on_init) {
			load_texture(key);
		}
	}

	AssetID load_texture(TEXTURE_KEY key) {
		auto id = asset_manager->load_texture(TEXTURE_PATH + path_from_texture_key[key]);
		loaded_textures[key] = id;
		return id;
	}
};