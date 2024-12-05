#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <containers/containers.h>
#include "SHP_events.h"
#include "window.h"
#include "renderer.h"
#include "io_manager.h"
#include "gui_manager.h"
#include "input_manager.h"
#include <iostream>
#include <string>

struct UserInterfaceInitData {
    WindowInitData win_data;
};

class UserInterface {
private:
    // owns
    std::shared_ptr<Window> _window;
    std::shared_ptr<Renderer> _renderer;
    std::shared_ptr<IOManager> _io_manager;
    std::shared_ptr<GUIManager> _gui_manager;
    std::shared_ptr<InputManager> _input_manager;
    // runtime
    bool initialized = false;

public:
    // getters
    std::shared_ptr<Window>& window() { return _window; };
    std::shared_ptr<Renderer>& renderer() { return _renderer; };
    std::shared_ptr<IOManager>& io_manager() { return _io_manager; };
    std::shared_ptr<GUIManager>& gui_manager() { return _gui_manager; };
    std::shared_ptr<InputManager>& input_manager() { return _input_manager; };

    UserInterface() {
    }
    ~UserInterface() {
        IMG_Quit();
        SDL_Quit();
    }

    bool init(UserInterfaceInitData init_data) {
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
        // SDL_image
        if (IMG_Init(IMG_INIT_PNG) < 0) {
            IMG_Quit();
            SDL_Quit();
            _io_manager->print_line_error("SDL_image - " + std::string(IMG_GetError()), FAILED_TO_INITIALIZE);
            return false;
        }

        _io_manager = std::make_shared<IOManager>();
        _input_manager = std::make_shared<InputManager>();

        _window = std::make_shared<Window>(_io_manager, init_data.win_data);
        if (_window->init()) {
            return false;
        }

        _renderer = std::make_shared<Renderer>(_io_manager, _window);
        if (_renderer->init()) {
            return false;
        }

        _gui_manager = std::make_shared<GUIManager>(_io_manager, _window, _renderer);
        if (_gui_manager->init(GUIManagerInitParams{})) {
            return false;
        }

        initialized = true;
        return true;
    }
};

