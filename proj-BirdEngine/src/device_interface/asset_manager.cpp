#include <asset_manager.h>
#include <core/bird_engine.h>

namespace WING_API {
	AssetID AssetManager::load_texture(AssetPath path) {
		return WING::Wing::instance()->device_interface()->asset_manager()->load_texture(path);
	}
}
