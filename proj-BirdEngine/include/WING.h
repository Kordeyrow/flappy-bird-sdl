#pragma once
#include "declspec.h"
#include "window.h"
#include "renderer.h"
#include "registry.h"
#include "asset_manager.h"
//#include "containers.h"
#include <memory> 

namespace BIRDENGINE_API {
    struct DECLSPEC Wing {
        Wing(const Wing&) = delete;
        Wing& operator=(const Wing&) = delete;
        static std::shared_ptr<Wing> instance();

        bool init();
        void lauch();
        static void run_game();
        //static void close_game();

    private:
        Wing();
        ~Wing();
    };
}