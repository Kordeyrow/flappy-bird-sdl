#pragma once
#include <cinttypes>

struct Vector2 { public: int x, y; };
struct Size : public Vector2 { public: Size(int width = 0, int height = 0) : Vector2{ width, height } {} };
struct Position : public Vector2 { public: Position(int x = 0, int y = 0) : Vector2{ x, y } {} };
struct Direction : public Vector2 { public: Direction(int x = 0, int y = 0) : Vector2{ x, y } {} };
struct Rect { Size size; Position position; };
struct Color { public: uint8_t r; uint8_t g; uint8_t b; uint8_t a; };

