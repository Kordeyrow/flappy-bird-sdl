#pragma once
#include <SDL.h>
#include <memory>

namespace WING {
    class IRenderer {
    public:
        virtual SDL_Renderer* sdl_renderer() = 0;
    };

    class IRendererContainer {
    public:
        std::shared_ptr<IRenderer> renderer;
    };
}