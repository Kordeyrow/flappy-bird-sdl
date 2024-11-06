#pragma once
#include <algorithm>

class Vector2 {
public:
	double x, y = 0;

    Vector2 operator*(const Vector2& other) const {
        return Vector2{ x * other.x, y * other.y };
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2{ x + other.x, y + other.y };
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2{ x - other.x, y - other.y };
    }

    float magnitude()
    {
        return sqrt((x * x) + (y * y));
    }

    float distance(const Vector2& other) {
        Vector2 v{ abs(x - other.x), abs(x - other.x) };
        return v.magnitude();
    }
};