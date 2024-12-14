#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "containers.h"
#include "SHP_events.h"
#include "window.h"
#include "renderer.h"
#include "io_manager.h"
#include "gui_manager.h"
#include "asset_manager.h"
#include "input_manager.h"
#include <iostream>
#include <string>

struct UserInterfaceInitData {
    WindowInitData win_data;
};

class DeviceInterface {
private:
    // owns
    std::shared_ptr<Window> _window;
    std::shared_ptr<Renderer> _renderer;
    std::shared_ptr<IOManager> _io_manager;
    std::shared_ptr<GUIManager> _gui_manager;
    std::shared_ptr<AssetManager> _asset_manager;
    std::shared_ptr<InputManager> _input_manager;
    // runtime
    bool initialized = false;

public:
    // getters
    std::shared_ptr<Window>& window() { return _window; };
    std::shared_ptr<Renderer>& renderer() { return _renderer; };
    std::shared_ptr<IOManager>& io_manager() { return _io_manager; };
    std::shared_ptr<GUIManager>& gui_manager() { return _gui_manager; };
    std::shared_ptr<AssetManager>& asset_manager() { return _asset_manager; };
    std::shared_ptr<InputManager>& input_manager() { return _input_manager; };

    DeviceInterface() {
    }
    ~DeviceInterface() {
        SDL_Quit();
    }

    //bool init(UserInterfaceInitData init_data) {
    bool init() {
        if (initialized) {
            _io_manager->print_line_warning("DeviceManager", ALREADY_INITIALIZED);
            return true;
        };
        // SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            _io_manager->print_line_error("SDL - " + std::string(SDL_GetError()), FAILED_TO_INITIALIZE);
            SDL_Quit();
            return false;
        }

        _io_manager = std::make_shared<IOManager>();

        //_window = std::make_shared<Window>(_io_manager, init_data.win_data);
        _window = std::make_shared<Window>(_io_manager);
        if ( ! _window->init()) {
            return false;
        }

        std::shared_ptr<IRendererContainer> irenderer_container = std::make_shared<IRendererContainer>();

        _asset_manager = std::make_shared<AssetManager>(_io_manager, irenderer_container);
        if (!_asset_manager->init()) {
            return false;
        }

        _renderer = std::make_shared<Renderer>(_io_manager, _window, _asset_manager);
        if ( ! _renderer->init()) {
            return false;
        }
        irenderer_container->renderer = std::shared_ptr<IRenderer>(_renderer);

        _gui_manager = std::make_shared<GUIManager>(_io_manager, _window, _renderer);
        if ( ! _gui_manager->init(GUIManagerInitParams{})) {
            return false;
        }

        _input_manager = std::make_shared<InputManager>();

        initialized = true;
        return true;
    }

    ProgramState update(float elapsed_time_seconds) {
        return _input_manager->update(elapsed_time_seconds);
    }
};

