#pragma once
#include "declspec.h"
#include "containers.h"

namespace WING_API {
	class DECLSPEC AssetManager {
	public:
		AssetID load_texture(AssetPath path);
	};
}

