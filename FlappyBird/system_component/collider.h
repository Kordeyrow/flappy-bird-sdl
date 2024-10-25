#pragma once
#include <math/vector2.h>
#include <system_component/entity.h>
#include <system_component/transform.h>

class Collider {
public:
	Collider(Entity* owner, Transform* t)
		: _owner{ owner},
		collider_transform{ t } {}

	virtual void collided(Collider* other) = 0;

	Entity* owner() const {
		return _owner;
	}

	bool is_colliding(Collider* other) const {
		Transform* other_transform = other->collider_transform;

		if (collider_transform->position.x < other_transform->position.x + other_transform->size.x &&
			collider_transform->position.x + collider_transform->size.x > other_transform->position.x &&
			collider_transform->position.y < other_transform->position.y + other_transform->size.y &&
			collider_transform->position.y + collider_transform->size.y > other_transform->position.y) {
			return true;
		}

		return false;
	}

	Transform* collider_transform;

private:
	Entity* _owner;
	bool _just_collided = false;
};

