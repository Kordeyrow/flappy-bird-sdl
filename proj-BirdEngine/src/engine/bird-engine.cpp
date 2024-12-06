#include "dll/pch.h"
#include "dll/framework.h"
#include "bird-engine.h"
#include <user_interface/gui_manager.h>
#include <iostream>
#include <memory>
#include <mutex>

struct BirdEngine::Impl {
    std::shared_ptr<UserInterface> user_interface = std::make_shared<UserInterface>();
    bool initialized = false;
    Impl() {}
    ~Impl() = default;

    bool init(EngineInitData init_data) {
        if (initialized) return true;
        if (user_interface->init(init_data.user_interface_init_data)) {
            return false;
        };
        initialized = true;
        return true;
    }

    ProgramState run() {
        if ( ! initialized) return ProgramState::QUIT;

        float elapsed_time_seconds = calculate_elapsed_time_seconds();
        //user_interface->input_manager()->update();

        ProgramState state = read_input();
        if (state == ProgramState::QUIT) {
            return state;
        }
        user_interface->renderer()->draw();

        return state;
    }

    float calculate_elapsed_time_seconds() {
        static uint32_t previous_time = 0;
        auto current_time = get_current_time();
        auto elapsed_time_seconds = (current_time - previous_time) / 1000.0f; // Convert to seconds.
        previous_time = current_time;
        return elapsed_time_seconds;
    }

    uint32_t get_current_time() { 
        return SDL_GetTicks64(); 
    }

    ProgramState read_input() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            SDL_Keycode keycode = e.key.keysym.sym;
            switch (e.type) {
            case SDL_QUIT:
                return ProgramState::QUIT;
            }
        }
        return ProgramState::RUNNING;
    }
};

BirdEngine::BirdEngine() : pImpl(std::make_unique<Impl>()) {}
BirdEngine::~BirdEngine() {};

std::shared_ptr<BirdEngine> BirdEngine::instance() {
    static std::shared_ptr<BirdEngine> instance(
        new BirdEngine(),
        [](BirdEngine* engine) {
            delete engine; // Custom deleter to allow destruction of the singleton
        }
    );
    return instance;
}

const std::shared_ptr<UserInterface>& BirdEngine::user_interface() { return  pImpl->user_interface; }

bool BirdEngine::init(EngineInitData init_data) {
    return pImpl->init(init_data);
}

ProgramState BirdEngine::run() {
    return pImpl->run();
}
