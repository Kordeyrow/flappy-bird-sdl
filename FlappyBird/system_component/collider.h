#pragma once
#include <math/vector2.h>
#include <system_component/entity.h>
#include <system_component/transform.h>

class Collider {
public:
	Collider(Entity* owner, Transform* t, Vector2 relative_scale)
		: _owner{ owner }, transform{ t }, relative_scale{ relative_scale } {}

	virtual void collided(Collider* other) = 0;

	Entity* owner() const {
		return _owner;
	}

    bool is_colliding(Collider* other) const {
        Transform* other_transform = other->transform;

        // Calculate the scaled collision transform size
        Vector2 scaled_size_ = calc_scaled_size();

        // Calculate the bounding box for this collider
        float x1 = transform->position.x - scaled_size_.x / 2;
        float y1 = transform->position.y - scaled_size_.y / 2;
        float w1 = scaled_size_.x;
        float h1 = scaled_size_.y;

        // Calculate the bounding box for the other collider
        Vector2 other_scaled_size_ = other->calc_scaled_size();
        float x2 = other_transform->position.x - other_scaled_size_.x / 2;
        float y2 = other_transform->position.y - other_scaled_size_.y / 2;
        float w2 = other_scaled_size_.x;
        float h2 = other_scaled_size_.y;

        // AABB (Axis-Aligned Bounding Box) collision check
        if (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2) {
            return true; // Colliders are intersecting
        }

        return false; // No collision
    }

    Vector2 calc_scaled_size() const {
        return transform->size * relative_scale;
    }

	Transform* transform;
	Vector2 relative_scale;

private:
	Entity* _owner;
	bool _just_collided = false;
};

