#pragma once
// libs
#include <SDL.h>
// cpp
#include <memory>
#include <algorithm>
#include <cinttypes>
// internal
#include "window.h"
#include "irenderer.h"
#include "asset_manager.h"
#include "containers.h"
#include <ecs/component/drawable.h>

namespace WING {
    using DrawableID = uint32_t;

    class Renderer : public IRenderer {
    private:
        // config
        Color default_renderer_draw_color = Color::WHITE();
        // dependency
        std::shared_ptr<AssetManager> asset_manager;
        std::shared_ptr<IOManager> io_manager;
        std::shared_ptr<Window> window;
        // runtime
        SDL_Renderer* _renderer;
        Color background_color = Color::BLUE_BIRD();

        std::vector<Drawable> drawable_list;
        bool initialized = false;

    public:
        Renderer(std::shared_ptr<IOManager> _io_manager, std::shared_ptr<Window> _window, std::shared_ptr<AssetManager> _asset_manager)
            : io_manager{ _io_manager }, window{ _window }, asset_manager{ _asset_manager } {
        }

        ~Renderer() {
            SDL_DestroyRenderer(_renderer);
            _renderer = nullptr;
        };

        bool init() {
            if (initialized) {
                io_manager->print_warning("Renderer", ALREADY_INITIALIZED);
                return true;
            }
            _renderer = SDL_CreateRenderer(window->sdl_window(), -1, SDL_RENDERER_ACCELERATED);
            if (!_renderer) {
                io_manager->print_error("Renderer - " + std::string(SDL_GetError()), FAILED_TO_CREATE);
                return false;
            }
            initialized = true;
            return true;
        }

        void draw() {
            draw_background();

            // draw textures
         //   std::sort(drawable_list.begin(), drawable_list.end(), less_than_compare_key_Drawable());
            //for (auto d : drawable_list) {
         //       auto rect = SDL_Rect{ d.transform->position.x, d.transform->position.y, d.transform->size.x, d.transform->size.y };
            //	SDL_RenderCopyEx(_renderer, asset_manager->get_texture(d.texture_id), NULL, &rect, d.transform->rotation, NULL, SDL_FLIP_NONE);
            //}
            // apply_draw
            SDL_RenderPresent(_renderer);
            SDL_RenderClear(_renderer);
        }

        void draw_drawable(Drawable d) {
            auto rect = SDL_Rect{ d.transform->position.x, d.transform->position.y, d.transform->size.x, d.transform->size.y };
            SDL_RenderCopyEx(_renderer, asset_manager->get_texture(d.texture_id), NULL, &rect, d.transform->rotation, NULL, SDL_FLIP_NONE);
        }

        void add_drawable(Drawable drawable) {
            drawable_list.push_back(drawable);
        }

        void set_background_color(Color c) {
            background_color = c;
        }

        // TODO: hide from user
        SDL_Renderer* sdl_renderer() {
            return _renderer;
        }

        void draw_background() {
            SDL_SetRenderDrawColor(
                _renderer,
                background_color.r,
                background_color.g,
                background_color.b,
                background_color.a
            );
            SDL_Rect r{
                0,
                0,
                window->rect().size.x,
                window->rect().size.y
            };
            SDL_RenderFillRect(_renderer, &r);
            reset_render_draw_color();
        }

    private:
        void reset_render_draw_color() {
            SDL_SetRenderDrawColor(
                _renderer,
                default_renderer_draw_color.r,
                default_renderer_draw_color.g,
                default_renderer_draw_color.b,
                default_renderer_draw_color.a);
        }
    };
}
