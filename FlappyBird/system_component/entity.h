#pragma once
#include <set>

enum TAG {
	PLAYER
};

class Entity {
public:
	Entity(std::set<TAG> tags = std::set<TAG>{}) : _tags{ tags } {}

	const std::set<TAG>& tags() const {
		return _tags;
	}
private:
	std::set<TAG> _tags;
};

