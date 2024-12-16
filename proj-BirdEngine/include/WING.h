#pragma once
#include "declspec.h"
#include "window.h"
#include "renderer.h"
#include "registry.h"
#include "asset_manager.h"
#include "input_manager.h"
//#include "containers.h"
#include <memory> 

namespace WING_API {
    struct DECLSPEC Wing {
        //Wing(const Wing&) = delete;
        //Wing& operator=(const Wing&) = delete;
        //static std::shared_ptr<Wing> instance();

        static bool init();
        static void lauch();
        static void run_game();
        //static void close_game();

    //private:
    //    Wing();
    //    ~Wing();
    };
}