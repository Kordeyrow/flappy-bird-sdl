#pragma once
#include <map>
#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include <utils/OpenGLDebugger.h>




class TextureManager {
public:
	enum TEXTURE_KEY {
		TEXTURE_FLAPPY_BIRD_UP_WING,
		PIPE
	};

	TextureManager() = default;

	bool load_init_textures() {
		for (TEXTURE_KEY key : textures_to_load_on_init) {
			if (load_texture(key) == 0) {
				return false;
			}
		}
		return true;
	}

	GLuint get_texture(TEXTURE_KEY key) {
		auto iterator = loaded_textures.find(key);
		if (iterator != loaded_textures.end()) {
			return iterator->second;
		}
		return load_texture(key);
	}

	void kill() {
		for (auto& t : loaded_textures) {
			glDeleteTextures(1, &t.second); // Delete OpenGL textures
		}
		loaded_textures.clear();
	}

private:
	// Config
	std::string SPRITES_PATH = "assets/sprites/";
	std::map<TEXTURE_KEY, std::string> textures_paths{
		{ TEXTURE_FLAPPY_BIRD_UP_WING, "yellowbird-upflap.png" },
		{ PIPE, "pipe-green.png" }
	};
	std::array<TEXTURE_KEY, 2> textures_to_load_on_init{
		TEXTURE_FLAPPY_BIRD_UP_WING
	};

	// Runtime
	std::map<TEXTURE_KEY, GLuint> loaded_textures;

	void CheckOpenGLError(const char* stmt, const char* fname, int line)
	{
		bool found_error = false;
		GLenum err = glGetError();
		//while (err) {
		//	if (err == GL_NO_ERROR) break;
		//	printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
		//	found_error = true;
		//	err = glGetError();
		//}
		//if (found_error) {
		//	//abort();
		//}
	}

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

	GLuint load_texture(TEXTURE_KEY key) {

		if (IMG_Init(IMG_INIT_PNG) == 0) {
			std::cerr << "a Failed to initialize SDL_image with PNG support: " << IMG_GetError() << std::endl;
			return 0;
		}

		std::string path = SPRITES_PATH + textures_paths[key];
		std::cout << ">> Loading texture from path: " << path << std::endl;

		SDL_Surface* s = IMG_Load(path.c_str());
		if (!s) {
			std::cerr << "c Error loading sprite: " << IMG_GetError() << std::endl;
			return 0;
		}
		
		SDL_Surface* optimized_surface = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_RGBA32, 0);
		if (!optimized_surface) {
			std::cerr << "Failed to convert surface to RGBA format: " << SDL_GetError() << std::endl;
			SDL_FreeSurface(s);
			return 0;
		}
		SDL_FreeSurface(s); // Free the original surface
		s = optimized_surface; // Use the converted surface

		if (!SDL_GL_GetCurrentContext()) {
			std::cerr << "Error: No active OpenGL context!" << std::endl;
			return 0;
		}

		//GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		glBindTexture(GL_TEXTURE_2D, 0);
		GLuint texture_id = 0;
		glGenTextures(1, &texture_id);
		if (texture_id == 0) {
			std::cerr << "d Failed to generate OpenGL texture!" << std::endl;
			return 0;
		};

		glBindTexture(GL_TEXTURE_2D, texture_id);
		//DebugLog(glBindTexture(GL_TEXTURE_2D, texture_id));
		//std::cout << s->
		

		int format = 0;
		if (s->format->BytesPerPixel == 4) {
			if (s->format->Rmask == 0x000000ff)
				format = GL_RGBA;
			else
				format = GL_BGRA;
		}
		else if (s->format->BytesPerPixel == 3) {
			if (s->format->Rmask == 0x000000ff)
				format = GL_RGB;
			else
				format = GL_BGR;
		}

		//GLenum format = GL_RGBA; // Or GL_RGB depending on image
		//if (surface->format->BytesPerPixel == 3) {
		//	format = GL_RGB;
		//}
		//GLenum internal_format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		//GLenum data_format = internal_format; // Match data format to internal format
		//int a = surface->format->BytesPerPixel;
		//std::cerr << a << std::endl;
		//if (surface->format->BytesPerPixel == 3) {
		//	data_format = GL_RGB;
		//}
		//else if (surface->format->BytesPerPixel == 4) {
		//	data_format = GL_RGBA;
		//}
		//else {
		//	std::cerr << "e Unsupported texture format: " << surface->format->BytesPerPixel << " bytes per pixel" << std::endl;
		//	SDL_FreeSurface(surface);
		//	return 0;
		//}

		int w = s->w;
		int h = s->h;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);

		// Set texture parameters
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
		
		
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

		// Free the SDL_Surface
		SDL_FreeSurface(s);

		// Store the texture ID
		loaded_textures[key] = texture_id;

		return texture_id;
	}
};

//class TextureManager {
//public:
//	enum TEXTURE_KEY {
//		TEXTURE_FLAPPY_BIRD_UP_WING,
//		PIPE
//	};
//	TextureManager(SDL_Renderer* _renderer)
//	{
//		renderer = _renderer;
//	}
//	bool load_init_textures()
//	{
//		for (TEXTURE_KEY key : textures_to_load_on_init)
//		{
//			if (load_texture(key) == nullptr) {
//				return false;
//			}
//		}
//		return true;
//	}
//	SDL_Texture* get_texture(TEXTURE_KEY key)
//	{
//		auto iterator = loaded_textures.find(key);
//		if (iterator != loaded_textures.end())
//			return iterator->second;
//		return load_texture(key);
//	}
//	void kill()
//	{
//		for (auto& t : loaded_textures) {
//			SDL_DestroyTexture(t.second);
//		}
//		loaded_textures.clear();
//	}
//
//private:
//	// config
//	std::string SPRITES_PATH = "assets/sprites/";
//	std::map<TEXTURE_KEY, std::string> textures_paths {
//		{ static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING), "yellowbird-upflap.png" },
//		{ static_cast<TEXTURE_KEY>(PIPE), "pipe-green.png" }
//	};
//	std::array<TEXTURE_KEY, 2> textures_to_load_on_init{
//		static_cast<TEXTURE_KEY>(TEXTURE_FLAPPY_BIRD_UP_WING),
//		static_cast<TEXTURE_KEY>(PIPE)
//	};
//	// shared
//	SDL_Renderer* renderer;
//	// runtime
//	std::map<TEXTURE_KEY, SDL_Texture*> loaded_textures;
//
//	SDL_Texture* load_texture(TEXTURE_KEY key)
//	{
//		//std::string path = textures_paths[key];
//		//std::cout << "Loading texture from path: " << path << std::endl; // Debugging path output
//
//		// Load bitmap into surface
//		//SDL_Surface* buffer = IMG_Load("C:\\dev\\FlappyBird\\FlappyBird\\FlappyBird\\src\\resources\\sprites\\yellowbird-upflap.png");
//		//SDL_Surface* buffer = IMG_Load(s2.c_str());
//		SDL_Surface* buffer = IMG_Load((SPRITES_PATH + textures_paths[key]).c_str());
//		//SDL_Surface* buffer = IMG_Load("../resources/sprites/yellowbird-upflap.png");
//		if (!buffer) {
//			std::cout << "Error loading sprite: " << SDL_GetError() << std::endl;
//			return nullptr;
//		}
//
//		// Create texture
//
//		/*buffer->format->Amask = 0xFF000000;
//		buffer->format->Ashift = 24;*/
//
//		/*Uint32 colorkey = SDL_MapRGB(buffer->format, 255, 255, 255);
//		SDL_SetColorKey(buffer, SDL_TRUE, colorkey);*/
//
//		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, buffer);
//		
//		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
//		//SDL_SetTextureAlphaMod(texture, 140);
//		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//
//		// Free surface as it's no longer needed
//		SDL_FreeSurface(buffer);
//		buffer = nullptr;
//		if (!texture) {
//			std::cout << "Error creating sprite: " << SDL_GetError() << std::endl;
//			return nullptr;
//		}
//
//		loaded_textures.emplace(key, texture);
//
//		return texture;
//	}
//};