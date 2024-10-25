#pragma once
#include <math/vector2.h>

class Transform {
public:
	Transform(Vector2 position, Vector2 size, double rotation = 0)
		: position{ position }, size{ size }, rotation { rotation } {}

	Vector2 position;
	Vector2 size;
	double rotation;
};

