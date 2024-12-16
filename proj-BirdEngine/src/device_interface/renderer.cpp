#include <renderer.h>
#include <engine_core.h>

namespace WING_API {
	void Renderer::set_background_color(Color c) {
		return WING::EngineCore::instance()->device_interface()->renderer()->set_background_color(c);
	}
}