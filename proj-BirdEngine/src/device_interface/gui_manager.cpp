#include "gui_manager.h"

// Macros for mapping keycodes

GUIManager::GUIManager(std::shared_ptr<IOManager> _io_manager, std::shared_ptr<Window> _window, std::shared_ptr<Renderer> _renderer)
    : io_manager{_io_manager}, window{ _window }, renderer{ _renderer } {
}
GUIManager::~GUIManager() {

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

bool GUIManager::init(const GUIManagerInitParams& params) {
    if (initialized) {
        io_manager->print_line_warning("GUIManager", ALREADY_INITIALIZED);
        return true;
    };
    if ( ! init_imgui(window->sdl_window(), renderer->sdl_renderer())) {
        io_manager->print_line_error("ImGUI", FAILED_TO_INITIALIZE);
        return false;
    };;
    initialized = true;
    return true;
}

bool GUIManager::init_imgui(SDL_Window* window, SDL_Renderer* renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    return true;
}

