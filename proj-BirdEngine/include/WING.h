#pragma once
#include "declspec.h"
#include "registry.h"
#include "window.h"
#include "asset_manager.h"
//#include "containers.h"
#include <memory> // cpp

namespace BIRDENGINE_API {
    struct DECLSPEC BirdEngine {
        BirdEngine(const BirdEngine&) = delete;
        BirdEngine& operator=(const BirdEngine&) = delete;
        static std::shared_ptr<BirdEngine> instance();

        bool init();
        void lauch();

    private:
        BirdEngine();
        ~BirdEngine();
    };
}