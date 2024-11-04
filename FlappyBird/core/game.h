#pragma once
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include <functional>
#include <chrono>
#include <map>
#include <typeindex>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>
#include <SDL_image.h>

#include <entities/flappy_bird.h>
#include <res_manager/texture_manager.h>
#include <system_component/updatable.h>
#include <entities/pipe.h>
#include <state_machine_base/state_machine.h>
#include "game_state.h"

class Closed;
class Playing;

struct BoolCondition {
	bool& condition;

	BoolCondition(bool& cond) : condition(cond) {}

	bool evaluate() const {
		return condition;
	}
};

enum class ComparisonType {
	Greater,
	Less,
	Equal,
	NotEqual,
	GreaterOrEqual,
	LessOrEqual
};

template <typename T>
struct ValueCondition {
	T& lhs;
	ComparisonType comparison;
	const T* rhsValue = nullptr;
	T* rhsVariable = nullptr;

	ValueCondition(T& lhs, ComparisonType comp, const T& value)
		: lhs(lhs), comparison(comp), rhsValue(&value) {}

	ValueCondition(T& lhs, ComparisonType comp, T& rhsVar)
		: lhs(lhs), comparison(comp), rhsVariable(&rhsVar) {}

	bool evaluate() const {
		T rhs = rhsVariable ? *rhsVariable : *rhsValue;
		switch (comparison) {
		case ComparisonType::Greater: return lhs > rhs;
		case ComparisonType::Less: return lhs < rhs;
		case ComparisonType::Equal: return lhs == rhs;
		case ComparisonType::NotEqual: return lhs != rhs;
		case ComparisonType::GreaterOrEqual: return lhs >= rhs;
		case ComparisonType::LessOrEqual: return lhs <= rhs;
		}
		return false;
	}
};

struct TimerCondition {
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	float duration;

	TimerCondition(float durationInSeconds)
		: duration(durationInSeconds), startTime(std::chrono::steady_clock::now()) {}

	bool evaluate() const {
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = now - startTime;
		return elapsed.count() >= duration;
	}

	void reset() {
		startTime = std::chrono::steady_clock::now();
	}
};

struct EventCondition {
	bool& eventTriggered;

	EventCondition(bool& triggered) : eventTriggered(triggered) {}

	bool evaluate() const {
		return eventTriggered;
	}

	void reset() {
		eventTriggered = false;
	}
};





class StateTransitionBase {
public:
	StateTransitionBase() {}

	void addBoolCondition(bool& condition) {
		conditions.push_back([&condition]() { return BoolCondition(condition).evaluate(); });
	}

	template <typename T>
	void addValueCondition(T& lhs, ComparisonType comp, const T& rhsValue) {
		conditions.push_back([&lhs, comp, &rhsValue]() {
			return ValueCondition<T>(lhs, comp, rhsValue).evaluate();
			});
	}

	void addTimerCondition(float duration) {
		TimerCondition timer(duration);
		conditions.push_back([timer]() mutable { return timer.evaluate(); });
	}

	void addEventCondition(bool& eventTriggered) {
		conditions.push_back([&eventTriggered]() { return EventCondition(eventTriggered).evaluate(); });
	}

	bool isTriggered() const {
		for (const auto& condition : conditions) {
			if (!condition()) {
				return false;
			}
		}
		return true;
	}

private:
	std::vector<std::function<bool()>> conditions;
};

template<typename OriginState, typename TargetState>
class StateTransition : public StateTransitionBase {
};



class Game;
class GameOver;


//class GameTransitionsManager {
//private:
//	bool player_dead = false;
//	Game* game;
//	std::map<std::type_index, StateTransitionBase*> transitions;
//public:
//	GameTransitionsManager(Game* game) : game{game} {
//		auto& t = new StateTransition<Playing, GameOver>();
//		t->addBoolCondition(player_dead);
//		transitions[typeid(Playing)] = t;
//	}
//};


class Game {
public:
	// SDL
	SDL_Window* window() { return _window; }
	SDL_Renderer* renderer() { return _renderer; }
	// Managers 
	TextureManager* texture_manager() { return _texture_manager; }
	// Gameobjects
	std::vector<Drawable*>* sprites() { return &_sprites; }
	std::vector<Updatable*>* updatables() { return &_updatables; }
	// Window
	double window_w() const { return _window_w; }
	double window_h() const { return _window_h; }
	// Player
	FlappyBird* player() { return _player; }

	StateMachine* state_machine() { return _state_machine; }

	GameState* start_state;

private:
	// SDL
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	// Managers 
	TextureManager* _texture_manager;
	// Gameobjects
	std::vector<Drawable*> _sprites;
	std::vector<Updatable*> _updatables;
	// Window
	double _window_w;
	double _window_h;

	FlappyBird* _player;
	StateMachine* _state_machine{};

public:
	Game() {}

	template<typename StartStateType>
	void init() {
		// SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
			system("pause");
			return;
		}

		// IMG
		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {
			std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
			return;
		}

		// window
		_window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
		if (!_window) {
			std::cout << "Error creating window: " << SDL_GetError() << std::endl;
			system("pause");
			return;
		}

		// renderer
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
		if (!_renderer) {
			std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
			return;
		}
		//IMGUI_CHECKVERSION();
		//ImGui::CreateContext();
		ImGui::Begin("Game HUD");
		ImGui::Text("Score: %d", 1);
		ImGui::End();

		// window info
		_window_w = (double)SDL_GetWindowSurface(_window)->w;
		_window_h = (double)SDL_GetWindowSurface(_window)->h;

		// texture
		_texture_manager = new TextureManager(_renderer);
		_texture_manager->load_init_textures();

		// state machine
		_state_machine = new StateMachine();
		_state_machine->init(CreateState<StartStateType>());
	}

public:
	template<typename LastStateType>
	void run_until() {
		while (_state_machine->get_current_state()->is_type<LastStateType>()) {
			_state_machine->run();
		}
	}

	void spawn_player() {
		Vector2 player_size = Vector2{ 48, 36 };
		Vector2 player_start_pos = Vector2{ _window_w / 2, _window_h / 2 };
		_player = new FlappyBird{
			_texture_manager->get_texture(TextureManager::TEXTURE_FLAPPY_BIRD_UP_WING),
			_window_h,
			player_start_pos,
			player_size
		};
		_sprites.push_back(_player);
		_updatables.push_back(_player);
	}

	void spawn_pipe(double window_w, double window_h, float speed_x) {
		int height_bias = 0; // 50
		double random_height = 0; //200;
		int random_height_percent = 1;// rand() % 101 / 100;
		double height = 320;
		double gap_size = 140;
		Vector2 pipe_size = Vector2{ 64, height };
		Vector2 pipe_start_pos = Vector2{ window_w + pipe_size.x / 2, window_h / 2 + height_bias + pipe_size.y/2 + gap_size/2 + random_height * random_height_percent };

		Pipe* pipe_down = new Pipe{
			_state_machine,
			_texture_manager->get_texture(TextureManager::PIPE),
			pipe_start_pos,
			pipe_size,
			speed_x
		};
		_sprites.push_back(pipe_down);
		_updatables.push_back(pipe_down);

		Pipe* pipe_up = new Pipe{
			_state_machine,
			_texture_manager->get_texture(TextureManager::PIPE),
			pipe_start_pos + Vector2{0, -height - gap_size},
			pipe_size,
			speed_x,
			SDL_FLIP_VERTICAL
		};
		_sprites.push_back(pipe_up);
		_updatables.push_back(pipe_up);
	}

	uint32_t get_current_time()
	{
		return SDL_GetTicks64();
	}

	static void sort_sprites_by_layer(std::vector<Drawable*>& sprites) {
		std::sort(sprites.begin(), sprites.end(), [](Drawable* a, Drawable* b) {
			return a->layer_index() > b->layer_index();
			});
	}

	void draw_sprites()
	{
		sort_sprites_by_layer(_sprites);

		for (auto s : _sprites) {
			auto rect = s->get_rect();
			SDL_RenderCopyEx(_renderer, s->get_texture(), NULL, &rect, s->get_rotation(), NULL, s->flip());
		}
	}

	void draw_backgroung()
	{
		// Clear the window to white
		SDL_SetRenderDrawColor(_renderer, 120, 200, 250, 255);
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		SDL_Rect r{ 0, 0, w, h };
		SDL_RenderFillRect(_renderer, &r);
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	}

	void render() {
		draw_backgroung();
		draw_sprites();
		SDL_RenderPresent(_renderer);
	}

	void close() {
		SDL_DestroyWindow(_window);
		_window = nullptr;

		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;

		_texture_manager->kill();
		_texture_manager = nullptr;

		IMG_Quit();
		SDL_Quit();
	}

	template<typename GameStateType>
	GameState* CreateState() {
		return (GameState*) new GameStateType(this, _state_machine);
	}
};

class Menu : public GameState {
public:
	Menu(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {

	};

	State* run() override {
		return this;
	};

	void exit() override {

	};
};

class Closed : public GameState {
public:
	Closed(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {

	};

	State* run() override {
		return this;
	};

	void exit() override {

	};
};

class Closing : public GameState {
public:
	Closing(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {
		game->close();
	};

	State* run() override {
		return game->CreateState<Closed>();
	};

	void exit() override {

	};
};

class GameOver;

class Playing : public GameState, public GameplayBase {
private:
	//// CONFIG
	const float speed_x = -100;
	const double spawn_gap = 0.0145;
	double real_spawn_gap_seconds = spawn_gap * abs(speed_x);
	//// STATE
	// spawn pipe
	double current_spawn_timer_seconds = 0;
	// game loop
	bool quit = false;
	double total_elapsed_time = 0;
	uint32_t previous_time = 0;
public:
	Playing(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {
		// clear
		game->sprites()->clear();
		game->updatables()->clear();

		// spawn pipe
		current_spawn_timer_seconds = 0;
		game->spawn_pipe(game->window_w(), game->window_h(), speed_x);

		// player
		game->spawn_player();
		game->player()->jump();

		// game loop
		total_elapsed_time = 0;
		previous_time = game->get_current_time();
	};

	State* run() override {
		// time
		auto current_time = game->get_current_time();
		auto elapsed_time_seconds = (current_time - previous_time) / 1000.0f; // Convert to seconds.
		previous_time = current_time;
		// input
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			SDL_Keycode keycode = e.key.keysym.sym;
			switch (e.type) {
			case SDL_QUIT:
				return game->CreateState<Closing>();
			case SDL_KEYDOWN:
				if (keycode == SDLK_SPACE) {
					if (game->player()->dead() == false) {
						game->player()->jump();
					}
					if (game->player()->fallen()) {
						// reset
					}
				}
				break;
			case SDL_KEYUP:
				break;
			}
		}

		// update
		for (auto u : *game->updatables()) {
			u->update(elapsed_time_seconds);
		}

		// spawn pipes
		current_spawn_timer_seconds += elapsed_time_seconds;
		if (current_spawn_timer_seconds >= real_spawn_gap_seconds) {
			current_spawn_timer_seconds = 0;
			game->spawn_pipe(game->window_w(), game->window_h(), speed_x);
		}

		// physics
		total_elapsed_time += elapsed_time_seconds;
		if (total_elapsed_time > 0.02) {
			total_elapsed_time -= 0.02;

			// get colliders
			std::vector<Collider*> cols;
			for (auto u : *game->updatables()) {
				Collider* col = dynamic_cast<Collider*>(u);
				if (col) {
					cols.push_back(col);
				}
			}

			// check collision
			for (size_t i = 0; i < cols.size(); i++)
			{
				for (size_t j = i + 1; j < cols.size(); j++)
				{
					if (cols[i]->is_colliding(cols[j])) {
						cols[i]->collided(cols[j]);
						cols[j]->collided(cols[i]);
					}
				}
			}
		}

		//reder
		game->render();

		if (game->player()->dead()) {
			return game->CreateState<GameOver>();
		}
		return this;
	};

	void exit() override {

	};
};

class GameOver : public GameState {
public:
	GameOver(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {

	};

	State* run() override {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			SDL_Keycode keycode = e.key.keysym.sym;
			switch (e.type) {
				case SDL_QUIT:
					return game->CreateState<Closing>();
				case SDL_KEYDOWN:
					if (keycode == SDLK_SPACE) {
						return game->CreateState<Playing>();
					}
					break;
			}
		}
		return this;
	};

	void exit() override {

	};
};

//class GameStateFactory {
//public:
//	template<typename T>
//	static GameState* CreateState() {
//		return (GameState*) new T(this, _state_machine);
//	}
//};