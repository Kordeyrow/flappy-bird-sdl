#pragma once
#include "declspec.h"
#include "containers.h"

namespace BIRDENGINE_API {
	struct DECLSPEC Window {
		static void set_rect(Rect rect);
		static void set_title(std::string title);
		static const Rect& rect();
		static const Rect& display_rect();
	};
}