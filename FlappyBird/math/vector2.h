#pragma once
class Vector2 {
public:
	double x, y = 0;

    Vector2 operator*(const Vector2& other) const {
        return Vector2{ x * other.x, y * other.y };
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2{ x + other.x, y + other.y };
    }
};

