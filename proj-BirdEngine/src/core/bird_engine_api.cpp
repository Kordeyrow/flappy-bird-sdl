#include <WING.h>
#include <thread>
#include "core/bird_engine.h"

namespace BIRDENGINE_API {
	Wing::Wing() {};
	Wing::~Wing() {};
	std::shared_ptr<Wing> Wing::instance() {
		static bool initialized = false;
		static std::shared_ptr<Wing> instance(
			new Wing(),
			[](Wing* reg) {
				delete reg; // Custom deleter to allow destruction of the singleton
			}
		);
		if ( ! initialized) {
			instance->init();
		}
		return instance;
	}

	bool Wing::init() {
		return true;
	}

	void Wing::lauch() {
	}

	void run() {

	}

	std::thread run_game_t;
	bool running = false;
	bool closing = false;
	void Wing::run_game()	{
		BirdEngine::instance()->update();
		//if (running) return;
		//running = true;
		//run_game_t = std::thread{ run, "Hello" };
		//while ( ! closing) {
		//	BirdEngine::instance()->update();
		//}
	}

	//void Wing::close_game() {
	//	if ( ! running) return;
	//	closing = true;
	//}
}
