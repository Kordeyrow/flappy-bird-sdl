#pragma once
#include <cinttypes>
#include <vector>
#include <map>
//#include <utils/container/sparse_set.h>

struct Vector2 { public: int x, y; };
struct Direction : public Vector2 { public: Direction(int x = 0, int y = 0) : Vector2{ x, y } {} };
struct Size : public Vector2 { public: Size(int width = 0, int height = 0) : Vector2{ width, height } {} };
struct Position : public Vector2 { public: Position(int x = 0, int y = 0) : Vector2{ x, y } {} };
struct Rect { Position position; Size size; };

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

using GameObjectID = uint32_t;
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
public:
	Position position;
	Size size;
	float rotation = 0;

	Transform(GameObjectID owner_id, Position pos, Size sz, float rot)
		: position(pos), size(sz), rotation(rot), Component { owner_id } {}
};

using AssetID = uint32_t;
class Sprite : public Component {
public:
	GameObjectID owner_id;
	AssetID texture_id;
	int layer_index = 0;
	Sprite(GameObjectID owner_id, AssetID texture_id)
		: texture_id{ texture_id }, owner_id{ owner_id }, Component{ owner_id } {}
};

class GameObject { 
private:
	GameObjectID _id = next_id();
	std::vector<Component*> _component_list = { new Transform {_id, Position{10, 10}, Size{10, 10}, 0} };
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
		
		// Check if a component of type T already exists and is unique
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

using SceneID = uint32_t;
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