#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <user_interface/gui/gui_manager.h>
#include <device/device_init_data.h>
#include <iostream>

class DeviceManager {
	// Window
	double window_h;
	double window_w;
	// Renderer
	// GUI
	GUIManager gui_manager;

public:
	SDL_Window* window;
	SDL_Renderer* renderer;

	~DeviceManager() {
		// TODO:
		close();
	}

	bool init(DeviceInitData data) {
		// SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			print_line_error("Error initializing SDL: " + std::string(SDL_GetError()));
			return false;
		}
		// SDL_image
		if (IMG_Init(IMG_INIT_PNG) < 0) {
			print_line_error("Error initializing SDL_image: " + std::string(IMG_GetError()));
			return false;
		}

		// window
		window = SDL_CreateWindow("Flappy Bird", 0, 0, 0, 0, SDL_WINDOW_SHOWN);
		if (!window) {
			print_line_error("Failed to create window: " + std::string(SDL_GetError()));
			SDL_Quit();
			return false;
		}

		// renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			print_line_error("Failed to create renderer: " + std::string(SDL_GetError()));
			SDL_DestroyWindow(window);
			SDL_Quit();
			return false;
		}

		gui_manager.init(window, renderer);
		init_window(data.window_init_data);

		// TODO: move this setup to "client"
		//ImFont* score_font = gui_manager.add_font("assets/fonts/flappy-bird-score-font.ttf", 30.0f);

		return true;
	}
	void set_window_rect(WindowRect rect) {
		SDL_SetWindowSize(window, rect.size.x, rect.size.y);
		SDL_SetWindowPosition(window, rect.position.x, rect.position.y);
	}

private:
	void close() {
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;

		/*_texture_manager->kill();
		_texture_manager = nullptr;*/

		gui_manager.close();
		IMG_Quit();
		SDL_Quit();
	}

	void init_window(WindowInitData data) {
		set_window_rect(data.rect);
		SDL_SetWindowTitle(window, data.title.c_str());
	}

public: 
	void print(std::string msg) {
		std::cout << msg;
	}
	void print_line(std::string msg) {
		print(msg);
		std::cout << std::endl;
	}
	void print_line_error(std::string msg) {
		std::cerr << msg;
		std::cerr << std::endl;
	}
};

