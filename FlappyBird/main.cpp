#include <iostream>
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
	FlappyBird player{
		texture_manager->get_texture(TextureManager::TEXTURE_FLAPPY_BIRD_UP_WING),
		player_start_pos,
		player_size
	};
	player.jump();
	sprites.push_back(&player);
	updatables.push_back(&player);

	// pipe
	int height_bias = 150;
	Vector2 pipe_size = Vector2{ 64, 320 };
	Vector2 pipe_start_pos = Vector2{ window_w +pipe_size.x /2, window_h /2 -pipe_size.y /2 +height_bias };
	Pipe pipe{
		texture_manager->get_texture(TextureManager::PIPE),
		pipe_start_pos,
		pipe_size
	};
	sprites.push_back(&pipe);
	updatables.push_back(&pipe);

	// game loop
	uint32_t previous_time = get_current_time();
    while (true) {
		// time
		auto current_time = get_current_time();
		auto elapsed_time = (current_time - previous_time) / 1000.0f; // Convert to seconds.
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
                        player.jump();
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
			u->update(elapsed_time);
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

	//texture_manager = new TextureManager(renderer);

	////// load init_resources
	//// 
	//// init vectors_reserve
	//sprite_list.reserve(10);
	////
	//if (texture_manager->load_init_textures() == false) {
	//	return false;
	//}

	//physics.init();

	//// create bird_player
	//bird_player = new BirdPlayer(texture_manager, Vector2{ 0, 0 }, Vector2{ 40, 40 });
	//auto bird_player_go = &(Gameobject&)bird_player;
	//auto bird_player_sp = bird_player_go->get_component<Sprite>();
	//physics.add((Rigidbody*)bird_player);
	//enabled_gameobjects.push_back(bird_player_go);
	//// TODO: create sprite factory (to centralize "push_back")
	//if (bird_player_sp) {
	//	sprite_list.push_back(bird_player_sp);
	//}
	//// TODO: 

	//return true;
}

void draw_sprites()
{
	// TODO: sort by z index
	//
	for (auto s : sprites) {
		auto& rect = s->get_rect();
		SDL_RenderCopyEx(renderer, s->get_texture(), NULL, &rect, s->get_rotation(), NULL, SDL_FLIP_NONE);
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