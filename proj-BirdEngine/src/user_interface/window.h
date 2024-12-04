#pragma once
#include <SDL.h>
#include "io_manager.h"
#include <containers/containers.h>
#include <memory>

class WindowInitData {
public:
    std::string title;
    Rect rect;
};

class Window {
private:
    // config
    std::shared_ptr<IOManager> io_manager;
    std::string title;
    Rect _rect;
    // runtime
    SDL_Window* _window;
    bool initialized = false;

public:
    Window(std::shared_ptr<IOManager> _io_manager, WindowInitData init_data) 
        : io_manager{ _io_manager }, title{ init_data.title }, _rect{init_data.rect } 
    {
    }

    ~Window() { 
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    bool init() {
        if (initialized) return true;
        _window = SDL_CreateWindow("Flappy Bird",
            _rect.position.x, 
            _rect.position.y,
            _rect.size.x,
            _rect.size.y,
            SDL_WINDOW_SHOWN
        );
        if ( ! _window) {
            io_manager->print_line_error("Failed to create window: " + std::string(SDL_GetError()));
            return false;
        }
        initialized = true;
        return true;
    }

    const Rect& rect() {
        return _rect;
    }

    SDL_Window* sdl_window() {
        return _window;
    }
};

