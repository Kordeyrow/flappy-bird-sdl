#pragma once
#include <string>

struct Vector2 {
public:
    int x, y;
};

struct Size : public Vector2 { public: Size(int width = 0, int height = 0) : Vector2{ width, height } {} };
struct Position : public Vector2 { public: Position(int x = 0, int y = 0) : Vector2{ x, y } {} };
struct Direction : public Vector2 { public: Direction(int x = 0, int y = 0) : Vector2{ x, y } {} };

struct WindowRect {
    Size size;
    Position position;
};

struct WindowInitData {
public:
    std::string title;
    WindowRect rect;
};

struct DeviceInitData {
public:
    WindowInitData window_init_data;
};