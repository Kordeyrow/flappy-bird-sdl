#pragma once
#include <cinttypes>

struct Vector2 { public: int x, y; };
struct Size : public Vector2 { public: Size(int width = 0, int height = 0) : Vector2{ width, height } {} };
struct Position : public Vector2 { public: Position(int x = 0, int y = 0) : Vector2{ x, y } {} };
struct Direction : public Vector2 { public: Direction(int x = 0, int y = 0) : Vector2{ x, y } {} };
struct Rect { Size size; Position position; };
struct Color {
	static constexpr Color RED() { return Color(255, 0, 0); }
	static constexpr Color GREEN() { return Color(0, 255, 0); }
	static constexpr Color BLUE() { return Color(0, 0, 255); }
	static constexpr Color BLACK() { return Color(0, 0, 0); }
	static constexpr Color WHITE() { return Color(255, 255, 255); }
	static constexpr Color BLUE_BIRD() { return Color(120, 200, 250); }

	constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		: r(r), g(g), b(b), a(a)
	{}

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};
