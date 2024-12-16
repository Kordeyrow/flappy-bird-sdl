#include "dll/pch.h"
#include "dll/framework.h"
#include <WING.h>
#include "bird_engine.h"
#include <device_interface/gui_manager.h>
#include <iostream>
#include <memory>
#include <mutex>

namespace WING_API {
    Wing::Wing() {};
    Wing::~Wing() {};
    std::shared_ptr<Wing> Wing::instance() {
        static bool initialized = false;
        static std::shared_ptr<Wing> instance(
            new Wing(),
            [](Wing* reg) {
                delete reg; // Custom deleter to allow destruction of the singleton
            }
        );
        if (!initialized) {
            instance->init();
        }
        return instance;
    }

    bool Wing::init() {
        return true;
    }

    void Wing::lauch() {
    }

    void run() {

    }

    std::thread run_game_t;
    bool running = false;
    bool closing = false;
    void Wing::run_game() {
        WING::Wing::instance()->update();
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
    class PhysicsSystemComponent {

    };

    class PhysicsSystem {

    };

    class RenderSystemComponent : Component {
    public:
        RenderSystemComponent(GameObjectID owner_id)
            : Component{ owner_id } {}
        virtual AssetID texture_id() = 0;
        virtual Transform* transform() = 0;
        virtual int layer_index() = 0;
    };

    struct less_than_compare_key_RenderSystemComponent {
        inline bool operator() (RenderSystemComponent* d1, RenderSystemComponent* d2) {
            return (d1->layer_index() < d2->layer_index());
        }
    };

    class RenderSystem {
    private:
        std::shared_ptr<DeviceInterface> device_interface;
    public:
        RenderSystem(std::shared_ptr<DeviceInterface> _device_interface)
            : device_interface{ _device_interface } {}

        void update(std::vector<RenderSystemComponent*> comps) {

            device_interface->renderer()->draw();

            //device_interface->renderer()->draw_background();

            //std::sort(comps.begin(), comps.end(), less_than_compare_key_RenderSystemComponent());
            //for (RenderSystemComponent* c : comps)
            //{
            //    auto d = Drawable{ c->texture_id(), c->transform(), c->layer_index() };
            //    device_interface->renderer()->draw_drawable(d);
            //}

            //device_interface->renderer()->apply_draw();
        }
    };

    struct Wing::Impl {
        std::shared_ptr<DeviceInterface> device_interface = std::make_shared<DeviceInterface>();
        bool initialized = false;
        bool game_initialized = false;
        RenderSystem render_system{ device_interface };
        Scene* current_scene = nullptr;
        Impl() {}
        ~Impl() = default;

        //bool init(EngineInitData init_data) {
        bool init() {
            if (initialized) return true;
            //if (!device_interface->init(init_data.device_interface_init_data)) {
            if (!device_interface->init()) {
                return false;
            };
            initialized = true;
            return true;
        }

        void init_game() {
            //current_scene = Registry::instance()->init_start_scene();
        }

        ProgramState update() {
            if (game_initialized) {
                init_game();
            }

            if (!initialized) return ProgramState::QUIT;

            float elapsed_time_seconds = calculate_elapsed_time_seconds();

            ProgramState state = device_interface->update(elapsed_time_seconds);
            if (state == ProgramState::QUIT) {
                return state;
            }

            std::vector<RenderSystemComponent*> comps;

            if (current_scene != nullptr) {
                for (auto& go : current_scene->gameobject_list) {
                    auto* comp = go->get_component<RenderSystemComponent>();
                    if (comp) {
                        comps.push_back(comp);
                    }
                }
            }

            render_system.update(comps);
            //device_interface->renderer()->draw();

            return ProgramState::RUNNING;
        }

        float calculate_elapsed_time_seconds() {
            static uint32_t previous_time = 0;
            auto current_time = get_current_time();
            auto elapsed_time_seconds = (current_time - previous_time) / 1000.0f; // Convert to seconds.
            previous_time = current_time;
            return elapsed_time_seconds;
        }

        uint32_t get_current_time() {
            return SDL_GetTicks64();
        }
    };

    Wing::Wing() : pImpl(std::make_unique<Impl>()) {}
    Wing::~Wing() {};

    std::shared_ptr<Wing> Wing::instance() {
        static bool initialized = false;
        static std::shared_ptr<Wing> instance(
            new Wing(),
            [](Wing* engine) {
                delete engine; // Custom deleter to allow destruction of the singleton
            }
        );
        if (!initialized) {
            instance->init();
        }
        return instance;
    }

    const std::shared_ptr<DeviceInterface>& Wing::device_interface() { return  pImpl->device_interface; }

    bool Wing::init() {
        return pImpl->init();
    }
    //bool BirdEngine::init(EngineInitData init_data) {
    //    return pImpl->init(init_data);
    //}

    ProgramState Wing::update() {
        return pImpl->update();
    }
}