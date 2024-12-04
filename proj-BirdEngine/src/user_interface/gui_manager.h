#pragma once
#include <dll/export_def.h>
#include <SDL.h>
#include "imgui_sdl/imgui.h"
#include "imgui_sdl/imgui_impl_sdl2.h"
#include "imgui_sdl/imgui_impl_sdlrenderer2.h"
#include "SHP_events.h"
#include "window.h"
#include "renderer.h"

struct GUIManagerInitParams {};

class BIRDENGINE_API GUIManager {
private:
    // dependency
    std::shared_ptr<IOManager> io_manager;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Window> window;
    // runtime
    bool initialized = false;

public:
    GUIManager(std::shared_ptr<IOManager> io_manager, std::shared_ptr<Window> _window, std::shared_ptr<Renderer> _renderer);
    ~GUIManager();

    bool init(const GUIManagerInitParams& params);

private:
    bool GUIManager::init_imgui(SDL_Window* window, SDL_Renderer* renderer);
};

