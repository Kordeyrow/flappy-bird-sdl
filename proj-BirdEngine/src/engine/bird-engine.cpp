#include "dll/pch.h"
#include "dll/framework.h"
#include "bird-engine.h"
#include <user_interface/gui_manager.h>
#include <iostream>
#include <memory>
#include <mutex>

struct BirdEngine::Impl {
    std::shared_ptr<DeviceInterface> device_interface;
    std::shared_ptr<GUIManager> gui_manager;
    std::shared_ptr<InputManager> _input_manager;
    bool initialized = false;

    Impl() {}
    ~Impl() = default;

    void init(EngineInitData init_data) {
        if (initialized) return;
        device_interface->init(init_data.device_init_data);
        initialized = true;
    }

    PROGRAM_STATE run() {
        // elapsed time
        float elapsed_time_seconds = calculate_elapsed_time_seconds();
        _input_manager->update(device_interface->get_events());
        //gui_manager.update_events();

        PROGRAM_STATE state = read_input();
        if (state == PROGRAM_STATE::QUIT) {
            return state;
        }

        draw();

        return state;
    }

    float calculate_elapsed_time_seconds() {
        static uint32_t previous_time = 0;
        auto current_time = get_current_time();
        auto elapsed_time_seconds = (current_time - previous_time) / 1000.0f; // Convert to seconds.
        previous_time = current_time;
        return elapsed_time_seconds;
    }

    uint32_t get_current_time()
    {
        return SDL_GetTicks64();
    }

    PROGRAM_STATE read_input() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            SDL_Keycode keycode = e.key.keysym.sym;
            switch (e.type) {
            case SDL_QUIT:
                return PROGRAM_STATE::QUIT;
            }
        }
        return PROGRAM_STATE::RUNNING;
    }
    InputManager& ui_manager() {
        return _input_manager;
    }
};

BirdEngine& BirdEngine::instance() {
    static BirdEngine instance;
    return instance;
}
InputManager& BirdEngine::ui_manager()
{
    return pImpl->ui_manager();
}

BirdEngine::BirdEngine() : pImpl(std::make_unique<Impl>()) {}
BirdEngine::~BirdEngine() = default;
void BirdEngine::init(EngineInitData init_data) {
    pImpl->init(init_data);
}
PROGRAM_STATE BirdEngine::run() {
    return pImpl->run();
}

Size BirdEngine::get_display_size()
{
    SDL_Rect r;
    SDL_GetDisplayBounds(0, &r);
    return Size(r.w, r.h);
}

void BirdEngine::set_window_rect(WindowRect rect)
{
    pImpl->device_interface.set_window_rect(rect);
}

void BirdEngine::set_background_color(Color c)
{
    return pImpl->set_background_color(c);
}
