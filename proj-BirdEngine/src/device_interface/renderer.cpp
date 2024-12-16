#include <renderer.h>
#include <core/bird_engine.h>

namespace BIRDENGINE_API {
	void Renderer::set_background_color(Color c) {
		return BirdEngine::instance()->device_interface()->renderer()->set_background_color(c);
	}
}