#pragma once
#include <core/game.h>

int main(int argc, char** argv) {

	//ImGui::Text("Hello, world %d", 123);
	Game game{};
	game.init<Playing>();
	game.run_until<Closed>();

	return 0;
}