#pragma once
#include <dll/export_def.h>
#include <SDL.h>
#include "imgui_sdl/imgui.h"
#include "imgui_sdl/imgui_impl_sdl2.h"
#include "imgui_sdl/imgui_impl_sdlrenderer2.h"
#include "SHP_events.h"
#include "window.h"
#include "renderer.h"
#include "io_manager.h"

using FontID = Uint32;
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
    GUIManager(std::shared_ptr<IOManager> _io_manager, std::shared_ptr<Window> _window, std::shared_ptr<Renderer> _renderer);
    ~GUIManager();

    bool init(const GUIManagerInitParams& params);

    ImFont* add_font(std::string path, float size_pixels) {
        return ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), size_pixels);
    }

    void set_dark_colorstyle() {
        ImGui::StyleColorsDark();
    }

private:
    bool GUIManager::init_imgui(SDL_Window* window, SDL_Renderer* renderer);
};

