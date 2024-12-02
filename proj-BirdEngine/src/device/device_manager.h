#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <user_interface/gui/gui_manager.h>

struct DeviceInitData {
public:
	std::string title;
	SDL_Rect desired_rect;
};

class DeviceManager {
	// Window
	SDL_Window* window;
	double window_h;
	double window_w;
	// Renderer
	SDL_Renderer* renderer;
	// GUI
	GUIManager gui_manager;

public:
	~DeviceManager() {
		//close();
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

		// TODO: move this setup to "client"
		int w = 460;
		int h = 640;
		double offset_x = -w * 0.04;
		double offset_y = -offset_x * 0.6;
		int pos_x = w / 2 - w / 2 + offset_x;
		int pos_y = h / 2 - h / 2 + offset_y;
		setup_window(data.title, data.desired_rect);

		// TODO: move this setup to "client"
		gui_manager.set_dark_colorstyle();
		ImFont* score_font = gui_manager.add_font("assets/fonts/flappy-bird-score-font.ttf", 30.0f);

		return true;
	}

private:
	void close() {
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyWindow(window);
		window = nullptr;

		SDL_DestroyRenderer(renderer);
		renderer = nullptr;

		/*_texture_manager->kill();
		_texture_manager = nullptr;*/

		IMG_Quit();
		SDL_Quit();
	}

	void setup_window(std::string title, SDL_Rect desired_rect) {
		SDL_Rect display_rect;
		SDL_GetDisplayBounds(0, &display_rect);
		/*
		_window_h = r.h * _window_h_percent_from_client;
		_window_w = _window_h * _window_w_percent_from_h;
#ifdef __EMSCRIPTEN__
		_window_h = r.h;
		_window_w = r.w;
#endif
		*/
		window_w = 460;
		window_h = 640;

		SDL_SetWindowSize(window, window_w, window_h);
		double offset_x = -desired_rect.w * 0.04;
		double offset_y = -offset_x * 0.6;
		SDL_SetWindowPosition(window,
			desired_rect.w / 2 - window_w / 2 + offset_x,
			desired_rect.h / 2 - window_h / 2 + offset_y);
		SDL_SetWindowTitle(window, title.c_str());
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

