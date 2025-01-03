#include "dll/pch.h"
#include "dll/framework.h"
#include <wing.h>
#include "engine_core.h"
#include <device_interface/gui_manager.h>
#include <registry/registry.h>
#include <iostream>
#include <memory>
#include <mutex>

namespace WING_API {
    //Wing::Wing() {};
    //Wing::~Wing() {};
    //std::shared_ptr<EngineCore> EngineCore::instance() {
    //    static bool initialized = false;
    //    static std::shared_ptr<EngineCore> instance(
    //        new EngineCore(),
    //        [](EngineCore* reg) {
    //            delete reg; // Custom deleter to allow destruction of the singleton
    //        }
    //    );
    //    if (!initialized) {
    //        instance->init();
    //    }
    //    return instance;
    //}

    bool Wing::init() {
        return true;
    }

    void Wing::lauch() {
    }

    void run() {

    }

    //std::thread run_game_t;
    bool running = false;
    bool closing = false;
    void Wing::run_game() {
        WING::EngineCore::instance()->update();
        //if (running) return;
        //running = true;
        //run_game_t = std::thread{ run, "Hello" };
        //while ( ! closing) {
        //	BirdEngine::instance()->update();
        //}
    }

    //void Wing::close_game() {
    //	if ( ! running) return;
    //	closing = true;
    //}
}

namespace WING {

    class RenderSystem {
    private:
        std::shared_ptr<DeviceInterface> _device_interface;
    public:
        RenderSystem(std::shared_ptr<DeviceInterface> device_interface_)
            : _device_interface{ device_interface_ } {}

        void update(std::vector<RenderSystemComponent*> comps, float elapsed_time_seconds) {

            //_device_interface->renderer()->draw();

            // background
            _device_interface->renderer()->draw_background();

            // each Drawable
            std::sort(comps.begin(), comps.end(), less_than_compare_key_RenderSystemComponent());
            for (RenderSystemComponent* c : comps) {
                auto d = Drawable{ c->texture_id(), c->transform(), c->layer_index() };
                _device_interface->renderer()->draw_drawable(d);
            }

            // apply
            _device_interface->renderer()->apply_draw();
        }
    };

    class PsysicsSystem {
    private:
        const float WORLD_RATIO = 0.068;
        const float GRAVITY_FORCE = 9.8 * WORLD_RATIO;
        const float AIR_RESISTANCE = 1.02 * WORLD_RATIO;
        std::shared_ptr<DeviceInterface> _device_interface;
    public:
        PsysicsSystem(std::shared_ptr<DeviceInterface> device_interface_)
            : _device_interface{ device_interface_ } {}

        void update(std::vector<PhysicsSystemComponent*> comps, float elapsed_time_seconds) {
            for (PhysicsSystemComponent* c : comps) {
                // add gravity
                auto a = { c->velocity().x, c->velocity().y };
                c->set_velocity(c->velocity().x / AIR_RESISTANCE, c->velocity().y + GRAVITY_FORCE);
                // apply velocity
                c->transform()->rect().position += c->velocity() * elapsed_time_seconds;
            }
        }

        void apply_velocity(std::vector<PhysicsSystemComponent*> comps) {
            for (PhysicsSystemComponent* c : comps) {
                c->set_velocity(c->velocity().x / AIR_RESISTANCE, c->velocity().y + GRAVITY_FORCE);
            }
        }
    };

    std::shared_ptr<DeviceInterface> _device_interface = std::make_shared<DeviceInterface>();
    std::shared_ptr<WING::Registry> _registry = std::make_shared<WING::Registry>();
    bool initialized = false;
    bool game_initialized = false;
    PsysicsSystem psysics_system{ _device_interface };
    RenderSystem render_system{ _device_interface };
    Scene* current_scene = nullptr;

    //bool init(EngineInitData init_data) {
    bool EngineCore::init() {
        if (initialized) return true;
        //if (!device_interface->init(init_data.device_interface_init_data)) {
        if (!_device_interface->init()) {
            return false;
        };
        initialized = true;
        return true;
    }

    void EngineCore::init_game() {
        current_scene = _registry->start_scene();
        game_initialized = true;
    }

    ProgramState EngineCore::update() {
        if ( ! initialized) init();
        if ( ! game_initialized) init_game();

        float elapsed_time_seconds = calculate_elapsed_time_seconds();

        ProgramState state = _device_interface->update(elapsed_time_seconds);
        if (state == ProgramState::QUIT) {
            return state;
        }

        // physics
        std::vector<PhysicsSystemComponent*> p_comps;
        if (current_scene != nullptr) {
            for (auto& go : current_scene->gameobject_list) {
                auto* comp = go->get_component<PhysicsSystemComponent>();
                if (comp) {
                    p_comps.push_back(comp);
                }
            }
        }
        psysics_system.update(p_comps, elapsed_time_seconds);

        // render
        std::vector<RenderSystemComponent*> r_comps;
        if (current_scene != nullptr) {
            for (auto& go : current_scene->gameobject_list) {
                auto* comp = go->get_component<RenderSystemComponent>();
                if (comp) {
                    r_comps.push_back(comp);
                }
            }
        }
        render_system.update(r_comps, elapsed_time_seconds);

        //device_interface->renderer()->draw();

        return ProgramState::RUNNING;
    }

    uint32_t EngineCore::get_current_time() {
        return SDL_GetTicks64();
    }

    float EngineCore::calculate_elapsed_time_seconds() {
        static uint32_t previous_time = 0;
        auto current_time = get_current_time();
        auto elapsed_time_seconds = (current_time - previous_time) / 1000.0f; // Convert to seconds.
        previous_time = current_time;
        return elapsed_time_seconds;
    }

    EngineCore::EngineCore() {};
    EngineCore::~EngineCore() {};

    std::shared_ptr<EngineCore> EngineCore::instance() {
        static bool initialized = false;
        static std::shared_ptr<EngineCore> instance(
            new EngineCore(),
            [](EngineCore* engine) {
                delete engine; // Custom deleter to allow destruction of the singleton
            }
        );
        if (!initialized) {
            instance->init();
        }
        return instance;
    }

    const std::shared_ptr<DeviceInterface>& EngineCore::device_interface() { return _device_interface; }
    const std::shared_ptr<WING::Registry>& EngineCore::registry() { return _registry; }

    //bool BirdEngine::init(EngineInitData init_data) {
    //    return pImpl->init(init_data);
    //}
}