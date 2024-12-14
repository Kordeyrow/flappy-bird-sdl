#pragma once
#include <SDL.h>
#include "io_manager.h"
#include "containers.h"
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
    SDL_Window* _sdl_window;
    bool initialized = false;

public:
    Window(std::shared_ptr<IOManager> _io_manager, WindowInitData init_data) 
        : io_manager{ _io_manager }, title{ init_data.title }, _rect{init_data.rect } {
    }
    ~Window() { 
        SDL_DestroyWindow(_sdl_window);
        _sdl_window = nullptr;
    }

    bool init() {
        if (initialized) {
            io_manager->print_line_warning("Window", ALREADY_INITIALIZED);
            return true;
        }
        _sdl_window = SDL_CreateWindow(title.c_str(),
            _rect.position.x, 
            _rect.position.y,
            _rect.size.x,
            _rect.size.y,
            SDL_WINDOW_SHOWN
        );
        if ( !_sdl_window) {
            io_manager->print_line_error("Window - " + std::string(SDL_GetError()), FAILED_TO_CREATE);
            return false;
        }
        initialized = true;
        return true;
    }

    /// setters
    ///
    void set_rect(Rect rect_) {
        _rect = rect_;
        SDL_SetWindowSize(_sdl_window, _rect.size.x, _rect.size.y);
        SDL_SetWindowPosition(_sdl_window, _rect.position.x, _rect.position.y);
    }
    void set_title(std::string title_) {
        title = title_;
        SDL_SetWindowTitle(_sdl_window, title.c_str());
    }

    /// getters
    ///
    const Rect& rect() {
        return _rect;
    }
    const Rect& display_rect() {
        SDL_Rect r;
        SDL_GetDisplayBounds(0, &r);
        return Rect{ Position{r.x, r.y}, Size{r.w, r.h } };
    }

    SDL_Window* sdl_window() {
        return _sdl_window;
    }
};

