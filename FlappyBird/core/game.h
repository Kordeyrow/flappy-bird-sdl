#pragma once
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <entities/flappy_bird.h>
#include <res_manager/texture_manager.h>
#include <system_component/updatable.h>
#include <entities/pipe.h>
#include <state_machine_base/state_machine.h>
#include "game_state.h"


class GameContext {
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
	bool initialized = false;

public:
	bool init() {
		if (initialized)
			return true;

		// SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
			system("pause");
			return false;
		}

		// IMG
		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {
			std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
			return false;
		}

		// window
		_window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
		if (!_window) {
			std::cout << "Error creating window: " << SDL_GetError() << std::endl;
			system("pause");
			return false;
		}

		// renderer
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
		if (!_renderer) {
			std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
			return false;
		}

		// window info
		_window_w = (double)SDL_GetWindowSurface(_window)->w;
		_window_h = (double)SDL_GetWindowSurface(_window)->h;

		// texture
		_texture_manager = new TextureManager(_renderer);
		_texture_manager->load_init_textures();

		//spawn_player();

		// state machine
		_state_machine = new StateMachine();

		initialized = true;
		return true;
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

	template<typename T>
	static GameState* CreateState(GameContext* context, StateMachineEventEmitter* emitter) {
		return (GameState*) new T(context, emitter);
	}
};

class Menu : public GameState {
public:
	Menu(GameContext* context, StateMachineEventEmitter* emitter) : GameState{ context, emitter } {}

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
	Closed(GameContext* context, StateMachineEventEmitter* emitter) : GameState{ context, emitter } {}

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
	Closing(GameContext* context, StateMachineEventEmitter* emitter) : GameState{ context, emitter } {}

	void enter() override {
		context->close();
	};

	State* run() override {
		return new Closed(context, emitter);
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
	Playing(GameContext* context, StateMachineEventEmitter* emitter) : GameState{ context, emitter } {}

	void enter() override {
		// clear
		context->sprites()->clear();
		context->updatables()->clear();

		// spawn pipe
		current_spawn_timer_seconds = 0;
		context->spawn_pipe(context->window_w(), context->window_h(), speed_x);

		// player
		context->spawn_player();
		context->player()->jump();

		// game loop
		quit = false;
		total_elapsed_time = 0;
		previous_time = context->get_current_time();
	};

	State* run() override {
		// time
		auto current_time = context->get_current_time();
		auto elapsed_time_seconds = (current_time - previous_time) / 1000.0f; // Convert to seconds.
		previous_time = current_time;
		// input
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			SDL_Keycode keycode = e.key.keysym.sym;
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (keycode == SDLK_SPACE) {
					if (context->player()->dead() == false) {
						context->player()->jump();
					}
					if (context->player()->fallen()) {
						// reset
					}
				}
				break;
			case SDL_KEYUP:
				break;
			}
		}
		if (quit) {
			return new Closing(context, emitter);
		}

		// update
		for (auto u : *context->updatables()) {
			u->update(elapsed_time_seconds);
		}

		// spawn pipes
		current_spawn_timer_seconds += elapsed_time_seconds;
		if (current_spawn_timer_seconds >= real_spawn_gap_seconds) {
			current_spawn_timer_seconds = 0;
			context->spawn_pipe(context->window_w(), context->window_h(), speed_x);
		}

		// physics
		total_elapsed_time += elapsed_time_seconds;
		if (total_elapsed_time > 0.02) {
			total_elapsed_time -= 0.02;

			// get colliders
			std::vector<Collider*> cols;
			for (auto u : *context->updatables()) {
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
		context->render();

		if (context->player()->dead()) {
			return context->CreateState<GameOver>(context, emitter);
		}
		return this;
	};

	void exit() override {

	};
};

class GameOver : public GameState {
public:
	GameOver(GameContext* context, StateMachineEventEmitter* emitter) : GameState{ context, emitter } {}

	void enter() override {

	};

	State* run() override {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			SDL_Keycode keycode = e.key.keysym.sym;
			switch (e.type) {
				case SDL_KEYDOWN:
					if (keycode == SDLK_SPACE) {
						return new Playing(context, emitter);
					}
					break;
			}
		}
		return this;
	};

	void exit() override {

	};
};


class Game {
public:
	GameContext* context;
	Game() : context{ new GameContext() } {}
	void init() {
		context->init();
		context->state_machine()->init(new Playing{ context, context->state_machine() });
	}
	void run() {
		while (context->state_machine()->get_current_state()->is_type<Closed>()) {
			context->state_machine()->run();
		}
	}
};