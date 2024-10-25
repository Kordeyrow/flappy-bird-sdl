#pragma once
#include <math/vector2.h>
#include <system_component/entity.h>
#include <system_component/transform.h>

class Collider {
public:
	Collider(Entity* owner, Transform* t, Vector2 relative_size)
		: _owner{ owner }, transform{ t }, relative_size{ relative_size } {}

	virtual void collided(Collider* other) = 0;

	Entity* owner() const {
		return _owner;
	}

    bool is_colliding(Collider* other) const {
        Transform* other_transform = other->transform;

        // Calculate the scaled collision transform size
        Vector2 scaled_size = transform->size * relative_size;

        // Calculate the bounding box for this collider
        float x1 = transform->position.x;
        float y1 = transform->position.y;
        float w1 = scaled_size.x;
        float h1 = scaled_size.y;

        // Calculate the bounding box for the other collider
        float x2 = other_transform->position.x;
        float y2 = other_transform->position.y;
        float w2 = other_transform->size.x;
        float h2 = other_transform->size.y;

        // AABB (Axis-Aligned Bounding Box) collision check
        if (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2) {
            return true; // Colliders are intersecting
        }

        return false; // No collision
    }

	Transform* transform;
	Vector2 relative_size;

private:
	Entity* _owner;
	bool _just_collided = false;
};

