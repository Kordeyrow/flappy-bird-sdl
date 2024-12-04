#pragma once
#include <dll/export_def.h>
#include <user_interface/device_interface.h>
#include <containers/containers.h>
#include <user_interface/input_manager.h>
#include <memory>

typedef enum {
    RUNNING,
    QUIT
} PROGRAM_STATE;

struct EngineInitData {
public:
    DeviceInitData device_init_data;
};

// Exported class from the DLL
class BIRDENGINE_API BirdEngine {
public:
    // Deleted copy constructor and assignment operator
    BirdEngine(const BirdEngine&) = delete;
    BirdEngine& operator=(const BirdEngine&) = delete;

    // Static method to access the singleton instance
    static BirdEngine& instance();
    InputManager& ui_manager();

    void init(EngineInitData init_data);
    PROGRAM_STATE run();

    Size get_display_size();
    void set_window_rect(WindowRect rect);
    void set_background_color(Color c);

private:
    // Private constructor and destructor
    BirdEngine();
    ~BirdEngine();

    // Pimpl idiom: Pointer to the implementation
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
