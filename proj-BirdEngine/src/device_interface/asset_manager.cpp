#include <api/asset_manager.h>
#include <core/bird_engine.h>

AssetID AssetManagerAPI::load_texture(AssetPath path) {
    return BirdEngine::instance()->device_interface()->asset_manager()->load_texture(path);
}
