#include <api/window.h>
#include <core/bird_engine.h>

void WindowAPI::set_rect(Rect rect) {
	return BirdEngine::instance()->device_interface()->window()->set_rect(rect);
}

void WindowAPI::set_title(std::string title) {
	return BirdEngine::instance()->device_interface()->window()->set_title(title);
}

const Rect& WindowAPI::rect() {
	return BirdEngine::instance()->device_interface()->window()->rect();
}

const Rect& WindowAPI::display_rect() {
	return BirdEngine::instance()->device_interface()->window()->display_rect();
}
