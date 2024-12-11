#pragma once
#include <dll/export_def.h>
#include <containers/containers.h>
#include <memory>

struct BIRDENGINE_API BirdEngineAPI {
    BirdEngineAPI(const BirdEngineAPI&) = delete;
    BirdEngineAPI& operator=(const BirdEngineAPI&) = delete;
    static std::shared_ptr<BirdEngineAPI> instance();

    bool init();
    /*ProgramState update();
    const std::shared_ptr<DeviceInterface>& device_interface();*/

private:
    BirdEngineAPI();
    ~BirdEngineAPI();
};