// BirdEngine.cpp : Defines the exported functions for the DLL.
//

#include "dll/pch.h"
#include "dll/framework.h"
#include "bird-engine.h"
#include <iostream>

// This is the constructor of a class that has been exported.
BirdEngine::BirdEngine()
{
    return;
}

void BirdEngine::run()
{
    std::cout << "Engine run()" << std::endl;
    return;
}
