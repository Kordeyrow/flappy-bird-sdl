#pragma once
#include <dll/export_def.h>
#include <user_interface/user_interface.h>
#include <containers/containers.h>
#include <user_interface/input_manager.h>
#include <memory>

typedef enum {
    RUNNING,
    QUIT
} ProgramState;

struct EngineInitData {
public:
    UserInterfaceInitData user_interface_init_data;
};

class BIRDENGINE_API BirdEngine {
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
public:
    BirdEngine(const BirdEngine&) = delete;
    BirdEngine& operator=(const BirdEngine&) = delete;

    static std::shared_ptr<BirdEngine> instance();
    const std::shared_ptr<UserInterface>& user_interface();

    bool init(EngineInitData init_data);
    ProgramState run();
private:
    ~BirdEngine();
    BirdEngine();
};
