#pragma once
#include <device_interface/device_interface.h>
#include "containers.h"
#include <memory>
#include "registry.h"

struct EngineInitData {
public:
    UserInterfaceInitData device_interface_init_data;
};

class BirdEngine {
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;

public:
    BirdEngine(const BirdEngine&) = delete;
    BirdEngine& operator=(const BirdEngine&) = delete;
    static std::shared_ptr<BirdEngine> instance();

    //bool init(EngineInitData init_data);
    bool init();
    ProgramState update();

    const std::shared_ptr<DeviceInterface>& device_interface();

private:
    BirdEngine();
    ~BirdEngine();
};
