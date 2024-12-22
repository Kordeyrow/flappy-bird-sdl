#pragma once
#include "declspec.h"
#include "containers.h"

namespace WING_API {
	class DECLSPEC AssetManager {
	public:
		static AssetID load_texture(AssetPath path);
		static TextureData get_texture_data(AssetPath path);
	};
}

