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

    Impl() : device_manager(), user_interface() {}
    ~Impl() = default;

    void init(EngineInitData init_data) {
        device_manager.init(init_data.device_init_data);
    }

    PROGRAM_STATE run() {
        SDL_SetRenderDrawColor(device_manager.renderer, 120, 200, 250, 255);
        int w, h;
        SDL_GetWindowSize(device_manager.window, &w, &h);
        SDL_Rect r{ 0, 0, w, h };
        SDL_RenderFillRect(device_manager.renderer, &r);
        SDL_SetRenderDrawColor(device_manager.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(device_manager.renderer);
        SDL_RenderClear(device_manager.renderer);
        std::cout << "Engine run()" << std::endl;

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
