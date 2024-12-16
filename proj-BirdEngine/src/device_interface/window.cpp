#include <window.h>
#include <core/bird_engine.h>

namespace WING_API {
	void Window::set_rect(Rect rect) {
		return WING::Wing::instance()->device_interface()->window()->set_rect(rect);
	}

	void Window::set_title(std::string title) {
		return WING::Wing::instance()->device_interface()->window()->set_title(title);
	}

	const Rect& Window::rect() {
		return WING::Wing::instance()->device_interface()->window()->rect();
	}

	const Rect& Window::display_rect() {
		return WING::Wing::instance()->device_interface()->window()->display_rect();
	}
}