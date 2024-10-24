#include <iostream>
#include <map>
#include "SDL.h"
#include "flappy/flappy_bird.h"
#include "res_manager/texture_manager.h"

// func decl
bool init();
uint32_t get_current_time();
void draw_backgroung();
void draw_sprites();
void render();

// SDL
SDL_Window* window;
SDL_Renderer* renderer;
// Texture
TextureManager* texture_manager;
// Sprites
std::vector<Drawable*> sprites;

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

	init();
	texture_manager = new TextureManager(renderer);
	texture_manager->load_init_textures();

	Vector2 player_size = Vector2{ 30,30 };
	double window_w = (double)SDL_GetWindowSurface(window)->w;
	double window_h = (double)SDL_GetWindowSurface(window)->h;
	FlappyBird player{
		texture_manager->get_texture(TextureManager::TEXTURE_FLAPPY_BIRD_UP_WING),
		Vector2{ window_w / 2 - player_size.x / 2, window_h / 2 - player_size.y / 2 },
		player_size
	};

	sprites.push_back(&player);

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
        player.update(elapsed_time);

		render();
    }


	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	texture_manager->kill();
	texture_manager = nullptr;

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

void render()
{
	draw_backgroung();
	draw_sprites();
	SDL_RenderPresent(renderer);
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