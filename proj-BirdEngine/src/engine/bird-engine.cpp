#include "dll/pch.h"
#include "dll/framework.h"
#include "bird-engine.h"
#include <device/device_manager.h>
#include <user_interface/user_interface.h>
#include <iostream>
#include <mutex>

struct BirdEngine::Impl {
    DeviceManager device_manager;
    UserInterface user_interface;

    Impl() : device_manager(), user_interface() {}
    ~Impl() = default;

    void init() {
        //device_manager.init(data);
    }

    void run() {
        std::cout << "Engine run()" << std::endl;
    }
};

BirdEngine& BirdEngine::instance() {
    static BirdEngine instance;
    return instance;
}

BirdEngine::BirdEngine() : pImpl(std::make_unique<Impl>()) {}
BirdEngine::~BirdEngine() = default;

void BirdEngine::init() {
    pImpl->init();
}

void BirdEngine::run() {
    pImpl->run();
}