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

    class Wing {
    private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;

    public:
        Wing(const Wing&) = delete;
        Wing& operator=(const Wing&) = delete;
        static std::shared_ptr<Wing> instance();

        //bool init(EngineInitData init_data);
        bool init();
        ProgramState update();

        const std::shared_ptr<DeviceInterface>& device_interface();

    private:
        Wing();
        ~Wing();
    };
}
