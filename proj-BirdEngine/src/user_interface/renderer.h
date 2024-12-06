#pragma once
#include <SDL.h>
#include <containers/containers.h>
#include "window.h"
#include <memory>

class Renderer {
private:
    // config
    Color default_renderer_draw_color = Color::WHITE();
    // dependency
    std::shared_ptr<IOManager> io_manager;
    std::shared_ptr<Window> window;
    // runtime
    SDL_Renderer* _renderer;
    Color background_color = Color::BLUE_BIRD();
    bool initialized = false;

public:
    Renderer(std::shared_ptr<IOManager> _io_manager, std::shared_ptr<Window> _window)
        : io_manager{ _io_manager }, window{ _window } {
    }

    ~Renderer() {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    };

    bool init() {
        if (initialized) {
            io_manager->print_line_warning("Renderer", ALREADY_INITIALIZED);
            return true;
        }
        _renderer = SDL_CreateRenderer(window->sdl_window(), -1, SDL_RENDERER_ACCELERATED);
        if (!_renderer) {
            io_manager->print_line_error("Renderer - " + std::string(SDL_GetError()), FAILED_TO_CREATE);
            return false;
        }
        initialized = true;
        return true;
    }

    void draw() {
        draw_background();
        SDL_RenderPresent(_renderer);
        SDL_RenderClear(_renderer);
    }

    void set_background_color(Color c) {
        background_color = c;
    }

    // TODO: hide from user
    SDL_Renderer* sdl_renderer() {
        return _renderer;
    }

private:
    void reset_render_draw_color() {
        SDL_SetRenderDrawColor(_renderer, 
            default_renderer_draw_color.r, 
            default_renderer_draw_color.g, 
            default_renderer_draw_color.b, 
            default_renderer_draw_color.a);
    }

    void draw_background() {
        SDL_SetRenderDrawColor(_renderer,
            background_color.r,
            background_color.g,
            background_color.b,
            background_color.a
        );
        SDL_Rect r{ 0,
            0,
            window->rect().size.x,
            window->rect().size.y
        };
        SDL_RenderFillRect(_renderer, &r);
        reset_render_draw_color();
    }

};

