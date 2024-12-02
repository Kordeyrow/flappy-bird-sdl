#pragma once
#include <string>
#include <containers/containers.h>

class WindowRect : public Rect { 
public: 
    WindowRect(Size size = { 0,0 }, Position position = { 0,0 }) 
    : Rect{size, position} {} 
};

class WindowInitData {
public:
    std::string title;
    WindowRect rect;
};

class DeviceInitData {
public:
    WindowInitData window_init_data;
};
