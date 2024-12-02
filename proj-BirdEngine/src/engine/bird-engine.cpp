#include "dll/pch.h"
#include "dll/framework.h"
#include "bird-engine.h"
#include <iostream>

BirdEngine::BirdEngine() {}
BirdEngine::~BirdEngine() {}

void BirdEngine::init()
{
}

void BirdEngine::run()
{
    std::cout << "Engine run()" << std::endl;
    return;
}

UserInterface& BirdEngine::get_user_interface() {
    return user_interface;
}
