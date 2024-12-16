#include <renderer.h>
#include <core/bird_engine.h>

namespace WING_API {
	void Renderer::set_background_color(Color c) {
		return WING::Wing::instance()->device_interface()->renderer()->set_background_color(c);
	}
}