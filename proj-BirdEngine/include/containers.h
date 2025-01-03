#pragma once
// cpp
#include <cinttypes>
#include <vector>
#include <string>
#include <map>

struct Vector2 { 
public: 
	float x, y;
	Vector2() = default;
	Vector2(int _x, int _y)
		: x(_x), y(_y) {}
	Vector2(float _x, float _y)
		: x(_x), y(_y) {}
	Vector2& operator+= (const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vector2& operator*= (float val) {
		x *= val;
		y *= val;
		return *this;
	}

	Vector2 operator* (float mult) const {
		return { x * mult, y * mult };
	}

	Vector2 operator+ (const Vector2& other) const {
		return { x + other.x, y + other.y};
	}
};
//struct Direction : public Vector2 { 
//public:
//	Direction() = default;
//	Direction(float x = 0, float y = 0) : Vector2{ x, y } {}
//	Direction(int x = 0, int y = 0) : Vector2{ (float)x, (float)y } {}
//
//	std::istream& operator>>(std::istream& is, T& obj)
//	{
//		// read obj from stream
//		if (/* T could not be constructed */)
//			is.setstate(std::ios::failbit);
//		return is;
//	}
//}
using Direction = Vector2;
using Size = Vector2;
using Position = Vector2;

//struct Size : public Vector2 { 
//public: 
//	Size() = default;
//	Size(float width, float height) : Vector2{ width, height } {}
//	Size(int width, int height) : Vector2{ (float)width, (float)height } {}
//};
//struct Position : public Vector2 { 
//public: 
//	Position(float x = 0, float y = 0) : Vector2{ x, y } {}
//	Position(int x = 0, int y = 0) : Vector2{ (float)x, (float)y } {}
//};
struct Rect { 
	Position position; 
	Size size;
	float rotation = 0;
};

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


typedef enum {
	RUNNING,
	QUIT
} ProgramState;


using AssetPath = std::string;
using AssetID = uint32_t;
using GameObjectID = uint32_t;
using SceneID = uint32_t;


class TextureData {
private:
	Size _original_size;
public:
	TextureData() = default;
	TextureData(Size s) {
		_original_size = s;
	}
	const Size& original_size() { return _original_size; }
};


class Component {
protected:
	GameObjectID _owner_id;
	bool _unique_on_gameobject = false;
public:
	Component(GameObjectID owner_id_, bool _unique_on_gameobject = false)
		: _owner_id{ owner_id_ }, _unique_on_gameobject{ _unique_on_gameobject }{}
	virtual ~Component() = default;
	const GameObjectID& owner_id() const { return _owner_id; };
	const bool& unique_on_gameobject() const { return _unique_on_gameobject; };
};

class Transform : public Component {
private:
	Rect _rect;
public:
	Transform(GameObjectID owner_id, Rect rect)
		: _rect(rect), Component { owner_id } {}
	Rect& rect() { return _rect; }
};

class RenderSystemComponent : public Component {
public:
	RenderSystemComponent(GameObjectID owner_id)
		: Component{ owner_id } {}
	virtual AssetID texture_id() = 0;
	virtual Transform* transform() = 0;
	virtual int layer_index() = 0;
};

struct less_than_compare_key_RenderSystemComponent {
	inline bool operator() (RenderSystemComponent* d1, RenderSystemComponent* d2) {
		return (d1->layer_index() < d2->layer_index());
	}
};

		class PhysicsSystemComponent : public Component {
		public:
			PhysicsSystemComponent(GameObjectID owner_id)
				: Component{ owner_id } {}
			virtual Transform* transform() = 0;
			virtual Vector2 velocity() = 0;
			virtual void set_velocity(float x, float y) = 0;
		};

		class Collider : public Component {
		private:
			Transform* _transform;
			Size _size;
		public:
			Collider(GameObjectID owner_id, Transform* transform)
				: Component{ owner_id }, _transform{ transform } {}
			const Size& size() { return _size; }
			Transform* transform() { return _transform; }
		};

		class Rigidbody : public PhysicsSystemComponent {
			Direction _velocity{};
		public:
			Collider* _collider;
			Rigidbody(GameObjectID owner_id, Transform* transform)
				: PhysicsSystemComponent{ owner_id }, _collider{ new Collider{owner_id, transform} } {}
			Collider* collider() { return _collider; }
			Transform* transform() { return _collider->transform(); }
			Direction velocity() { return _velocity; }
			void set_velocity(float x, float y) { _velocity.x = x; _velocity.y = y; }


		};

class Sprite : public RenderSystemComponent {
public:
	Transform* _transform;
	AssetID _texture_id;
	int _layer_index = 0;
	Sprite(GameObjectID owner_id, Transform* transform, AssetID texture_id)
		: _texture_id{ texture_id }, _transform{ transform }, RenderSystemComponent{ owner_id } {}

	// RenderSystemComponent
	AssetID texture_id() {
		return _texture_id;
	}
	Transform* transform() {
		return _transform;
	}
	int layer_index() {
		return _layer_index;
	}
};

class GameObject { 
private:
	GameObjectID _id = next_id();
	std::vector<Component*> _component_list = { 
		new Transform {
			_id,
			Rect { 
				Position{10, 10},
				Size{10, 10},
				0,
			}
		}
	};

public:
	const GameObjectID& id() { return _id; };
	const std::vector<Component*>& component_list() { return _component_list; };
	GameObjectID next_id() {
		static GameObjectID current_id = 0;
		current_id++;
		return current_id;
	}

	template <typename T, typename ...Args>
	T* add_component(Args&&... args) {
		static_assert(std::is_base_of<Component, T>::value, "T must inherit Component");
		
		// Check if a component of type T already exists and should be unique
		for (Component* comp : _component_list) {
			T* t = dynamic_cast<T*>(comp);
			if (t && t->unique_on_gameobject()) {
				return nullptr;
			}
		}
		T* new_comp = new T(_id, std::forward<Args>(args)...);
		_component_list.emplace_back(new_comp);
		return new_comp;
	}

	template <typename T>
	T* get_component() const {
		for (size_t i = 0; i < _component_list.size(); i++) {
			T* t = dynamic_cast<T*>(_component_list[i]);
			if (t) {
				return t;
			}
		}
		return nullptr;
	}
};

class Scene {
private:
	SceneID _id = next_id();
public:
	std::vector<GameObject*> gameobject_list;
	const SceneID& id() { return _id; };
	SceneID next_id() {
		static SceneID current_id = 0;
		current_id++;
		return current_id;
	}

	void init() {

	}
};

typedef enum {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
} FOVBounds;

typedef enum {
	INSIDE,
	PARTIAL,
	OUTSIDE,
} FOVState;
