#include <asset_manager.h>
#include <engine_core.h>

namespace WING_API {
	AssetID AssetManager::load_texture(AssetPath path) {
		return WING::EngineCore::instance()->device_interface()->asset_manager()->load_texture(path);
	}
}
