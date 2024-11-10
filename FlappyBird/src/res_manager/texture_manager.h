#pragma once
#include <map>
#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class TextureManager {
public:
	enum TEXTURE_KEY {
		TEXTURE_FLAPPY_BIRD_UP_WING,
		PIPE
	};
	TextureManager(SDL_Renderer* _renderer)
	{
		renderer = _renderer;
	}
	bool load_init_textures()
	{
		for (TEXTURE_KEY key : textures_to_load_on_init)
		{
			if (load_texture(key) == nullptr) {
				return false;
			}
		}
		return true;
	}
	SDL_Texture* get_texture(TEXTURE_KEY key)
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
	std::string SPRITES_PATH = "assets/sprites/";
	std::map<TEXTURE_KEY, std::string> textures_paths {
		{ static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING), "yellowbird-upflap.png" },
		{ static_cast<TEXTURE_KEY>(PIPE), "pipe-green.png" }
	};
	std::array<TEXTURE_KEY, 2> textures_to_load_on_init{
		static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING),
		static_cast<TEXTURE_KEY>(PIPE)
	};
	// shared
	SDL_Renderer* renderer;
	// runtime
	std::map<TEXTURE_KEY, SDL_Texture*> loaded_textures;

	SDL_Texture* load_texture(TEXTURE_KEY key)
	{
		//std::string path = textures_paths[key];
		//std::cout << "Loading texture from path: " << path << std::endl; // Debugging path output

		// Load bitmap into surface
		//SDL_Surface* buffer = IMG_Load("C:\\dev\\FlappyBird\\FlappyBird\\FlappyBird\\src\\resources\\sprites\\yellowbird-upflap.png");
		//SDL_Surface* buffer = IMG_Load(s2.c_str());
		SDL_Surface* buffer = IMG_Load((SPRITES_PATH + textures_paths[key]).c_str());
		//SDL_Surface* buffer = IMG_Load("../resources/sprites/yellowbird-upflap.png");
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