#pragma once
#include "declspec.h"
#include "containers.h"

namespace BIRDENGINE_API {
	class DECLSPEC AssetManager {
	public:
		AssetID load_texture(AssetPath path);
	};
}

