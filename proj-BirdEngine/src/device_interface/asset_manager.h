#pragma once
// libs
#include <SDL.h>
#include <SDL_image.h>
// cpp
#include <map>
#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <cinttypes>
// internal
#include "irenderer.h"
#include "io_manager.h"

namespace WING {
	constexpr AssetID INVAL_ID = -1;

	class AssetManager {
	private:
		// dependency
		std::shared_ptr<IOManager> io_manager;
		std::shared_ptr<IRendererContainer> renderer_container;

		// runtime
		std::map<AssetPath, AssetID> id_from_assetpath;
		std::map<AssetID, SDL_Texture*> texture_from_id;

	public:
		AssetManager(std::shared_ptr<IOManager> _io_manager, std::shared_ptr<IRendererContainer> _renderer_container)
			: io_manager{ _io_manager }, renderer_container{ _renderer_container } {
		}
		~AssetManager() {
			id_from_assetpath.clear();
			for (auto& t : texture_from_id) {
				SDL_DestroyTexture(t.second);
			}
			texture_from_id.clear();
			IMG_Quit();
		}

		SDL_Texture* get_texture(AssetID id) {
			if (texture_from_id.find(id) == texture_from_id.end()) {
				return nullptr;
			}
			return texture_from_id[id];
		}

		AssetID get_asset_id(AssetPath path) {
			if (id_from_assetpath.find(path) == id_from_assetpath.end()) {
				return INVAL_ID;
			}
			return id_from_assetpath[path];
		}

		bool init() {
			// SDL_image
			if (IMG_Init(IMG_INIT_PNG) < 0) {
				IMG_Quit();
				io_manager->print_error("SDL_image - " + std::string(IMG_GetError()), FAILED_TO_INITIALIZE);
				return false;
			}
			return true;
		}

		// config
		//std::string SPRITES_PATH = "assets/sprites/";
		/*std::map<TEXTURE_KEY, std::string> textures_paths{
			{ static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING), "yellowbird-upflap.png" },
			{ static_cast<TEXTURE_KEY>(PIPE), "pipe-green.png" }
		};
		std::array<TEXTURE_KEY, 2> textures_to_load_on_init{
			static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING),
			static_cast<TEXTURE_KEY>(PIPE)
		};*/
		// shared
		//SDL_Renderer* renderer;
		// runtime

		AssetID load_texture(AssetPath path) {
			if (id_from_assetpath.find(path) != id_from_assetpath.end()) {
				return id_from_assetpath[path];
			}

			SDL_Surface* buffer = IMG_Load(path.c_str());
			if (!buffer) {
				io_manager->print_error('"' + path + "\" - " + std::string(IMG_GetError()), FAILED_LOADING);
				return INVAL_ID;
			}

			/*buffer->format->Amask = 0xFF000000;
			buffer->format->Ashift = 24;
			Uint32 colorkey = SDL_MapRGB(buffer->format, 255, 255, 255);
			SDL_SetColorKey(buffer, SDL_TRUE, colorkey);*/

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_container->renderer->sdl_renderer(), buffer);
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			//SDL_SetTextureAlphaMod(texture, 140);
			SDL_SetRenderDrawBlendMode(renderer_container->renderer->sdl_renderer(), SDL_BLENDMODE_BLEND);
			SDL_FreeSurface(buffer);
			buffer = nullptr;

			if (!texture) {
				io_manager->print_error("sprite" + std::string(IMG_GetError()), FAILED_TO_CREATE);
				return INVAL_ID;
			}

			auto id = get_next_id();
			id_from_assetpath.emplace(path, id);
			texture_from_id.emplace(id, texture);
			return id;
		}

	private:
		AssetID get_next_id() {
			static AssetID next_id = 0;
			return ++next_id;
		}
	};
}