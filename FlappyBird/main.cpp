#pragma once
#include <core/game.h>

int main(int argc, char** argv) {
	while (true) {
		Game game{};
		game.run();
	}
}