#include <asset_manager.h>
#include <core/bird_engine.h>

namespace BIRDENGINE_API {
	AssetID AssetManager::load_texture(AssetPath path) {
		return BirdEngine::instance()->device_interface()->asset_manager()->load_texture(path);
	}
}
