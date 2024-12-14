#pragma once
// cpp
#include <memory>
// internal
#include "registry.h"
#include "window.h"
#include "declspec.h"
//#include <containers/containers.h>

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