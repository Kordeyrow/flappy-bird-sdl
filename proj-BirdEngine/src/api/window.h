#pragma once
#include <containers/containers.h>

class WindowAPI {
public:
	void set_rect(Rect rect);
	void set_title(std::string title);
	const Rect& rect();
	const Rect& display_rect();
};

