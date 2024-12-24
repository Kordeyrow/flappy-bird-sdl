#pragma once
#include <SDL.h>
#include "io_manager.h"
#include "containers.h"
#include <memory>
#ifdef min
#undef min
#endif
#include <algorithm>

namespace WING {
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
        //Window(std::shared_ptr<IOManager> _io_manager, WindowInitData init_data)
        Window(std::shared_ptr<IOManager> _io_manager)
            : io_manager{ _io_manager }, title{ "MyGame" }, _rect{ Rect{{100, 100}, {100, 100}} } {
        }
        ~Window() {
            SDL_DestroyWindow(_sdl_window);
            _sdl_window = nullptr;
        }

        bool init() {
            if (initialized) {
                io_manager->print_warning("Window", ALREADY_INITIALIZED);
                return true;
            }
            _sdl_window = SDL_CreateWindow(title.c_str(),
                _rect.position.x,
                _rect.position.y,
                _rect.size.x,
                _rect.size.y,
                SDL_WINDOW_SHOWN
            );
            if (!_sdl_window) {
                io_manager->print_error("Window - " + std::string(SDL_GetError()), FAILED_TO_CREATE);
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
            return Rect{ Vector2{(float)r.x, (float)r.y}, Vector2{(float)r.w, (float)r.h } };
        }

        SDL_Window* sdl_window() {
            return _sdl_window;
        }







        // TODO Camera Class


        FOVState CheckFOVState(const Rect& other) const {
            // Edges of _rect
            float left_ref = 0;
            float right_ref = _rect.size.x;
            float top_ref = 0;
            float bottom_ref = _rect.size.y;

            // Edges of other
            float left_other = other.position.x;
            float right_other = other.position.x + other.size.x;
            float top_other = other.position.y;
            float bottom_other = other.position.y + other.size.y;

            // Check if "other" is entirely outside of "_rect"
            if (right_other < left_ref || left_other > right_ref || bottom_other < top_ref || top_other > bottom_ref) {
                return OUTSIDE;
            }

            // Check if "other" is entirely inside "_rect"
            if (left_other >= left_ref && right_other <= right_ref && top_other >= top_ref && bottom_other <= bottom_ref) {
                return INSIDE;
            }

            // Otherwise, it's partially overlapping
            return PARTIAL;
        }

        // Determines the closest side of "_rect" to the center of "other"
        FOVBounds CheckClosestFOVBound(const Rect& other) const {
            // Center of "other"
            Vector2 other_center = other.position + (other.size * 0.5f);
            // Compute distances from the center of "other" to each side of "_rect"
            float dist_to_left = std::abs(other_center.x - _rect.position.x);
            float dist_to_right = std::abs(other_center.x - (_rect.position.x + _rect.size.x));
            float dist_to_top = std::abs(other_center.y - _rect.position.y);
            float dist_to_bottom = std::abs(other_center.y - (_rect.position.y + _rect.size.y));

            // Find the minimum distance
            float min_dist = std::min({ dist_to_left, dist_to_right, dist_to_top, dist_to_bottom });

            // Return the corresponding side
            if (min_dist == dist_to_left) return LEFT;
            if (min_dist == dist_to_right) return RIGHT;
            if (min_dist == dist_to_top) return TOP;
            return BOTTOM;
        }
    };
};

