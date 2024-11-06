#pragma once
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <math/vector2.h>
#include <system_component/entity.h>
#include <system_component/transform.h>

#include <functional>
#include <unordered_map>
#include <utility>
#include <typeinfo>

class Collider {
public:
	Collider(Entity* owner, Transform* t, Vector2 relative_scale)
		: _owner{ owner }, transform{ t }, relative_scale{ relative_scale } {}

	virtual void collided(Collider* other) = 0;

	Entity* owner() const {
		return _owner;
	}

    virtual double left() { return transform->position.x - calc_scaled_size().x / 2; };
    virtual double right() { return transform->position.x + calc_scaled_size().x / 2; };
    virtual double top() { return transform->position.y - calc_scaled_size().y / 2; };
    virtual double bottom() { return transform->position.y + calc_scaled_size().y / 2; };

    Vector2 calc_scaled_size() const {
        return transform->size * relative_scale;
    }

	Transform* transform;
	Vector2 relative_scale;

private:
	Entity* _owner;
	bool _just_collided = false;
};

class RectangleCollider : public Collider {
public:
    RectangleCollider(Entity* owner, Transform* t, Vector2 relative_scale)
        : Collider{ owner, t, relative_scale } {}
};


class CircleCollider : public Collider {
private:
    double _radius;
public:
    CircleCollider(Entity* owner, Transform* t, float radius)
        : Collider(owner, t, Vector2{ 1, 1 }), _radius{ radius } {};

    double radius() const { return _radius; }

    virtual double left() { return transform->position.x - radius() / 2; };
    virtual double right() { return transform->position.x + radius() / 2; };
    virtual double top() { return transform->position.y - radius() / 2; };
    virtual double bottom() { return transform->position.y + radius() / 2; };
};

static class CollisionChecker {
public:
    static bool is_colliding(const CircleCollider& c1, const CircleCollider& c2) {
        float dx = c1.transform->position.x - c2.transform->position.x;
        float dy = c1.transform->position.y - c2.transform->position.y;
        float distance_squared = dx * dx + dy * dy;
        float radius_sum = c1.radius() + c2.radius();
        return distance_squared <= (radius_sum * radius_sum);
    }

    static bool is_colliding(const RectangleCollider& a, const RectangleCollider& b) {
        const auto& r1 = static_cast<const RectangleCollider&>(a);
        const auto& r2 = static_cast<const RectangleCollider&>(b);
        Vector2 r1_scaled_size = r1.calc_scaled_size();
        Vector2 r2_scaled_size = r2.calc_scaled_size();
        return std::abs(r1.transform->position.x - r2.transform->position.x) <= (r1_scaled_size.x / 2 + r2_scaled_size.x / 2) &&
            std::abs(r1.transform->position.y - r2.transform->position.y) <= (r1_scaled_size.y / 2 + r2_scaled_size.y / 2);
    }

    static bool is_colliding(const CircleCollider& circle, const RectangleCollider& rect) {
        Vector2 rect_scaled_size = rect.calc_scaled_size();

        float closest_x = std::max(rect.transform->position.x - rect_scaled_size.x / 2,
            std::min(circle.transform->position.x, rect.transform->position.x + rect_scaled_size.x / 2));
        float closest_y = std::max(rect.transform->position.y - rect_scaled_size.y / 2,
            std::min(circle.transform->position.y, rect.transform->position.y + rect_scaled_size.y / 2));

        float distance_x = circle.transform->position.x - closest_x;
        float distance_y = circle.transform->position.y - closest_y;
        return (distance_x * distance_x + distance_y * distance_y) <= (circle.radius() * circle.radius());
    }
};