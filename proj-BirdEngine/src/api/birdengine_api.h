#pragma once
#include <dll/export_def.h>
#include <device_interface/device_interface.h>
#include <containers/containers.h>
#include <memory>

struct BIRDENGINE_API BirdEngine {
    BirdEngine(const BirdEngine&) = delete;
    BirdEngine& operator=(const BirdEngine&) = delete;
    static std::shared_ptr<BirdEngine> instance();
    bool init();
    ProgramState update();
    const std::shared_ptr<DeviceInterface>& device_interface();
};