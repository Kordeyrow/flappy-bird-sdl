#pragma once
class Game {
public:
	bool game_over() const {
		return _game_over;
	}

	void set_game_over() {
		_game_over = true;
	}

protected:
	bool _game_over = false;
};

