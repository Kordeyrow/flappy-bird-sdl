#pragma once
#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class TextureManager {
public:
	enum TEXTURE_PATH_KEY {
		TEXTURE_FLAPPY_BIRD_UP_WING,
		PIPE
	};
	TextureManager(SDL_Renderer* _renderer)
	{
		renderer = _renderer;
	}
	bool load_init_textures()
	{
		for (TEXTURE_PATH_KEY key : textures_to_load_on_init)
		{
			if (load_texture(key) == nullptr) {
				return false;
			}
		}
		return true;
	}
	SDL_Texture* get_texture(TEXTURE_PATH_KEY key)
	{
		auto iterator = loaded_textures.find(key);
		if (iterator != loaded_textures.end())
			return iterator->second;
		return load_texture(key);
	}
	void kill()
	{
		for (auto& t : loaded_textures) {
			SDL_DestroyTexture(t.second);
		}
		loaded_textures.clear();
	}

private:
	// config
	std::map<TEXTURE_PATH_KEY, const char*> textures_paths{
		{ static_cast<TEXTURE_PATH_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING), "../res/sprites/yellowbird-upflap.png" },
		{ static_cast<TEXTURE_PATH_KEY>(PIPE), "../res/sprites/pipe-green.png" }
	};
	std::array<TEXTURE_PATH_KEY, 2> textures_to_load_on_init{
		static_cast<TEXTURE_PATH_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING),
		static_cast<TEXTURE_PATH_KEY>(PIPE)
	};
	// shared
	SDL_Renderer* renderer;
	// runtime
	std::map<TEXTURE_PATH_KEY, SDL_Texture*> loaded_textures;

	SDL_Texture* load_texture(TEXTURE_PATH_KEY key)
	{
		// Load bitmap into surface
		SDL_Surface* buffer = IMG_Load(textures_paths[key]);
		if (!buffer) {
			std::cout << "Error loading sprite: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		// Create texture

		/*buffer->format->Amask = 0xFF000000;
		buffer->format->Ashift = 24;*/

		/*Uint32 colorkey = SDL_MapRGB(buffer->format, 255, 255, 255);
		SDL_SetColorKey(buffer, SDL_TRUE, colorkey);*/

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, buffer);
		
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		//SDL_SetTextureAlphaMod(texture, 140);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		// Free surface as it's no longer needed
		SDL_FreeSurface(buffer);
		buffer = nullptr;
		if (!texture) {
			std::cout << "Error creating sprite: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		loaded_textures.emplace(key, texture);

		return texture;
	}
};

