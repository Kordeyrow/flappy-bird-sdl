#include "api/bird_engine.h"
#include "core/bird_engine.h"

// TODO: use same class name but different namespace
//namespace API {}

BirdEngineAPI::BirdEngineAPI() {};
BirdEngineAPI::~BirdEngineAPI() {};
std::shared_ptr<BirdEngineAPI> BirdEngineAPI::instance() {
	static std::shared_ptr<BirdEngineAPI> instance(
		new BirdEngineAPI(),
		[](BirdEngineAPI* reg) {
			delete reg; // Custom deleter to allow destruction of the singleton
		}
	);
	return instance;
}

bool BirdEngineAPI::init()
{
	return true;
	//return BirdEngine::instance()->init();
}
