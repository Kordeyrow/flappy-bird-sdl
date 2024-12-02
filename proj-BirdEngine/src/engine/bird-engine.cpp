#include "dll/pch.h"
#include "dll/framework.h"
#include "bird-engine.h"
#include <device/device_manager.h>
#include <user_interface/user_interface.h>
#include <iostream>
#include <mutex>

struct BirdEngine::Impl {
    DeviceManager device_manager;
    UserInterface user_interface;
    Color default_renderer_draw_color = Color{ 255, 255, 255, 255 };
    Color background_color = Color::BLUE_BIRD();

    Impl() : device_manager(), user_interface() {}
    ~Impl() = default;

    void init(EngineInitData init_data) {
        device_manager.init(init_data.device_init_data);
    }

    PROGRAM_STATE run() {
        // elapsed time
        float elapsed_time_seconds = calculate_elapsed_time_seconds();

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

    void reset_render_draw_color() {
        SDL_SetRenderDrawColor(device_manager.renderer, default_renderer_draw_color.r, default_renderer_draw_color.g, default_renderer_draw_color.b, default_renderer_draw_color.a);
    }

    void draw_background() {
        SDL_SetRenderDrawColor(device_manager.renderer, background_color.r, background_color.g, background_color.b, background_color.a);
        int w, h;
        SDL_GetWindowSize(device_manager.window, &w, &h);
        SDL_Rect r{ 0, 0, w, h };
        SDL_RenderFillRect(device_manager.renderer, &r);
        reset_render_draw_color();
    }

    void draw() {
        draw_background();
        SDL_RenderPresent(device_manager.renderer);
        SDL_RenderClear(device_manager.renderer);
    }
     
    void set_background_color(Color c) {
        background_color = c;
    }
};

BirdEngine& BirdEngine::instance() {
    static BirdEngine instance;
    return instance;
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
    pImpl->device_manager.set_window_rect(rect);
}

void BirdEngine::set_background_color(Color c)
{
    return pImpl->set_background_color(c);
}
