#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include <map>
#include "SDL.h"
#include <SDL_image.h>
#include <entities/flappy_bird.h>
#include <res_manager/texture_manager.h>
#include <system_component/updatable.h>
#include <entities/pipe.h>

// func decl
bool init();
uint32_t get_current_time();
void draw_backgroung();
void draw_sprites();
void spawn_pipe(double w, double h, float speed_x);

// SDL
SDL_Window* window;
SDL_Renderer* renderer;
// Texture
TextureManager* texture_manager;
// Sprites
std::vector<Drawable*> sprites;
// Entities
std::vector<Updatable*> updatables;


int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

	// init
	init();
	texture_manager = new TextureManager(renderer);
	texture_manager->load_init_textures();

	// get window data
	double window_w = (double)SDL_GetWindowSurface(window)->w;
	double window_h = (double)SDL_GetWindowSurface(window)->h;

	// player
	Vector2 player_size = Vector2{ 48, 36 };
	Vector2 player_start_pos = Vector2{ window_w /2 -player_size.x /2, window_h /2 -player_size.y /2 };
	FlappyBird* player = new FlappyBird{
		texture_manager->get_texture(TextureManager::TEXTURE_FLAPPY_BIRD_UP_WING),
		player_start_pos,
		player_size
	};
	player->jump();
	sprites.push_back(player);
	updatables.push_back(player);

	// pipe
	float speed_x = -100;
	double spawn_gap = 0.0145;
	double real_spawn_gap_seconds = spawn_gap * abs(speed_x);
	double spawn_delay_seconds_count = 0;
	spawn_pipe(window_w, window_h, speed_x);

	// game loop
	uint32_t previous_time = get_current_time();
    while (true) {
		// time
		auto current_time = get_current_time();
		auto elapsed_time_seconds = (current_time - previous_time) / 1000.0f; // Convert to seconds.
		previous_time = current_time;

        // input
        SDL_Event e;
		bool static quit = false;
        while (SDL_PollEvent(&e) != 0) {
            SDL_Keycode k = e.key.keysym.sym;
            switch (e.type) {
                case SDL_QUIT:
					quit = true;
                    break; 
                case SDL_KEYDOWN:
                    if (k == SDLK_SPACE) {
                        player->jump();
                    }
                    break;
                case SDL_KEYUP:
                    break;
            }
        }
		if (quit) {
			break;
		}

        // update
		for (auto u : updatables) {
			u->update(elapsed_time_seconds);
		}
		// spawn pipes
		spawn_delay_seconds_count += elapsed_time_seconds;
		if (spawn_delay_seconds_count >= real_spawn_gap_seconds) {
			spawn_delay_seconds_count = 0;
			spawn_pipe(window_w, window_h, speed_x);
		}

		// physics
		static double total_elapsed_time = 0;
		total_elapsed_time += elapsed_time_seconds;
		if (total_elapsed_time > 0.02) {
			total_elapsed_time -= 0.02;

			// get colliders
			std::vector<Collider*> cols;
			for (auto u : updatables) {
				Collider* col = dynamic_cast<Collider*>(u);
				if (col) {
					cols.push_back(col);
				}
			}

			// check collision
			for (size_t i = 0; i < cols.size(); i++)
			{
				for (size_t j = i+1; j < cols.size(); j++)
				{
					if (cols[i]->is_colliding(cols[j])) {
						cols[i]->collided(cols[j]);
						cols[j]->collided(cols[i]);
					}
				}
			}
		}

		//reder
		draw_backgroung();
		draw_sprites();
		SDL_RenderPresent(renderer);
    }


	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	texture_manager->kill();
	texture_manager = nullptr;

	IMG_Quit();
    SDL_Quit();

    return 0;
}

void spawn_pipe(double window_w, double window_h, float speed_x) {
	int height_bias = 50;
	double random_height = 200;
	int random_height_percent = rand() % 101;
	double height = 320;
	double gap_size = 140;
	Vector2 pipe_size = Vector2{ 64, height };
	Vector2 pipe_start_pos = Vector2{ window_w +pipe_size.x /2, window_h /2 -pipe_size.y /2 +height_bias +random_height *random_height_percent /100 };
	
	Pipe* pipe_down = new Pipe{
		texture_manager->get_texture(TextureManager::PIPE),
		pipe_start_pos,
		pipe_size,
		speed_x
	};
	sprites.push_back(pipe_down);
	updatables.push_back(pipe_down);

	Pipe* pipe_up = new Pipe{
		texture_manager->get_texture(TextureManager::PIPE),
		pipe_start_pos + Vector2{0, -height -gap_size},
		pipe_size,
		speed_x,
		SDL_FLIP_VERTICAL
	};
	sprites.push_back(pipe_up);
	updatables.push_back(pipe_up);
}

uint32_t get_current_time()
{
	return SDL_GetTicks64();
}

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {
		std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

static void sort_sprites_by_layer(std::vector<Drawable*>& sprites) {
	std::sort(sprites.begin(), sprites.end(), [](Drawable* a, Drawable* b) {
		return a->layer_index() > b->layer_index();
		});
}

void draw_sprites()
{
	sort_sprites_by_layer(sprites);

	for (auto s : sprites) {
		auto rect = s->get_rect();
		SDL_RenderCopyEx(renderer, s->get_texture(), NULL, &rect, s->get_rotation(), NULL, s->flip());
	}
}

void draw_backgroung()
{
	// Clear the window to white
	SDL_SetRenderDrawColor(renderer, 120, 200, 250, 255);
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	SDL_Rect r{ 0, 0, w, h };
	SDL_RenderFillRect(renderer, &r);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}