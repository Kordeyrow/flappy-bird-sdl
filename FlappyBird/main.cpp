#pragma once
#include <core/game.h>

int main(int argc, char** argv) {

	Game game{};
	game.init<Playing>();
	game.run_until<Closed>();

	return 0;
}