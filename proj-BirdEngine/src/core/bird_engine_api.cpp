#include <WING.h>
//#include "core/bird_engine.h"

namespace BIRDENGINE_API {
	BirdEngine::BirdEngine() {};
	BirdEngine::~BirdEngine() {};
	std::shared_ptr<BirdEngine> BirdEngine::instance() {
		static bool initialized = false;
		static std::shared_ptr<BirdEngine> instance(
			new BirdEngine(),
			[](BirdEngine* reg) {
				delete reg; // Custom deleter to allow destruction of the singleton
			}
		);
		if ( ! initialized) {
			instance->init();
		}
		return instance;
	}

	bool BirdEngine::init() {
		return true;
	}

	void BirdEngine::lauch() {
	}
}
