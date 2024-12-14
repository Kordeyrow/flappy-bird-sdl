#include <window.h>
#include <core/bird_engine.h>

namespace BIRDENGINE_API {
	void Window::set_rect(Rect rect) {
		return BirdEngine::instance()->device_interface()->window()->set_rect(rect);
	}

	void Window::set_title(std::string title) {
		return BirdEngine::instance()->device_interface()->window()->set_title(title);
	}

	const Rect& Window::rect() {
		return BirdEngine::instance()->device_interface()->window()->rect();
	}

	const Rect& Window::display_rect() {
		return BirdEngine::instance()->device_interface()->window()->display_rect();
	}
}
