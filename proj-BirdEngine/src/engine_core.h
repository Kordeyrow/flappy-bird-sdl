#pragma once
#include <device_interface/device_interface.h>
#include "containers.h"
#include <memory>
#include <registry/registry.h>

namespace WING {
    struct EngineInitData {
    public:
        UserInterfaceInitData device_interface_init_data;
    };

    class EngineCore {
    public:
        // singleton
        EngineCore(const EngineCore&) = delete;
        EngineCore& operator=(const EngineCore&) = delete;
        static std::shared_ptr<EngineCore> instance();

        // getters
        const std::shared_ptr<DeviceInterface>& device_interface();
        const std::shared_ptr<Registry>& registry();

        //bool init(EngineInitData init_data);
        bool init();
        ProgramState update();

    private:
        EngineCore();
        ~EngineCore();
        void init_game();
        uint32_t get_current_time();
        float calculate_elapsed_time_seconds();
    };
}
