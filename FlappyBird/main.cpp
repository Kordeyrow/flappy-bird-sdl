#pragma once
#include <core/game.h>
#include <imgui.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

int main(int argc, char** argv) {

	//ImGui::Text("Hello, world %d", 123);
	Game game{};
	game.init<Playing>();
	game.run_until<Closed>();

	return 0;
}