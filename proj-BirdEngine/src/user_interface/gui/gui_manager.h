#pragma once
#include <string>
#include "imgui_sdl/imgui.h"
#include "imgui_sdl/imgui_impl_sdl2.h"
#include "imgui_sdl/imgui_impl_sdlrenderer2.h"

class GUIManager {
public:
	bool init(SDL_Window* window, SDL_Renderer* renderer) {
		return init_imgui(window, renderer);
	}

	bool init_imgui(SDL_Window* window, SDL_Renderer* renderer) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);

		// frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		return true;
	}

	ImFont* add_font(std::string path, float size_pixels) {
		return ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), size_pixels);
	}

	void set_dark_colorstyle() {
		ImGui::StyleColorsDark();
	}
};

