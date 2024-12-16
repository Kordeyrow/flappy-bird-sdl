#pragma once
#include <device_interface/device_interface.h>
#include "containers.h"
#include <memory>
#include "registry.h"

namespace WING {
    struct EngineInitData {
    public:
        UserInterfaceInitData device_interface_init_data;
    };

    class EngineCore {
    private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;

    public:
        EngineCore(const EngineCore&) = delete;
        EngineCore& operator=(const EngineCore&) = delete;
        static std::shared_ptr<EngineCore> instance();

        //bool init(EngineInitData init_data);
        bool init();
        ProgramState update();

        const std::shared_ptr<DeviceInterface>& device_interface();

    private:
        EngineCore();
        ~EngineCore();
    };
}
