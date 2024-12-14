#pragma once
// cpp 
#include <memory>
// internal
#include "window.h"

class DeviceInterfaceAPI {
public:
    std::shared_ptr<WindowAPI>& window() { return _window; };
    //std::shared_ptr<RendererAPI>& renderer() { return _renderer; };
    //std::shared_ptr<IOManagerAPI>& io_manager() { return _io_manager; };
    //std::shared_ptr<GUIManagerAPI>& gui_manager() { return _gui_manager; };
    //std::shared_ptr<AssetManagerAPI>& asset_manager() { return _asset_manager; };
    //std::shared_ptr<InputManagerAPI>& input_manager() { return _input_manager; };
};