#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include <core/game.h>

// Global variables for window and renderer
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Rectangle to draw
SDL_Rect rect;

// Flag to control the main loop
bool quit = false;

// Event handler
SDL_Event e;

// Function to initialize SDL and create window and renderer
bool init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create a window
    const int window_width = 800;
    const int window_height = 600;
    window = SDL_CreateWindow("SDL2 Red Rectangle",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Define the rectangle dimensions
    int rect_width = 200;
    int rect_height = 150;
    rect.w = rect_width;
    rect.h = rect_height;
    rect.x = (window_width - rect_width) / 2;
    rect.y = (window_height - rect_height) / 2;

    return true;
}

// Function to clean up SDL resources
void cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// The function that will be called each frame
void run() {
    // Handle events on the queue
    while (SDL_PollEvent(&e) != 0) {
        // User requests to quit
        if (e.type == SDL_QUIT) {
            quit = true;
#ifdef __EMSCRIPTEN__
            // For Emscripten, need to stop the main loop
            emscripten_cancel_main_loop();
#endif
        }
    }

    // Clear screen with white color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Set draw color to red for the rectangle
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Draw the rectangle
    SDL_RenderFillRect(renderer, &rect);

    // Update the screen
    SDL_RenderPresent(renderer);

#ifndef __EMSCRIPTEN__
    // For native compilation, check if we need to exit
    if (quit) {
        cleanUp();
        exit(0);
    }
#endif
}
Game game;

void initt() {
    game.init<Playing>();
}

void run_game() {
    quit = game.run_until<Closed>();
    if (quit) {
        exit(0);
    }
}

#ifdef __EMSCRIPTEN__
int main() {
    /*if (!init()) {
        return 1;
    }*/
    initt();

    // Ensure SDL is properly initialized and run the main loop
    emscripten_set_main_loop(run_game, 0, 1);

    // Cleanup will be called when the loop exits
    //cleanUp();

    return 0;
}
#else
int main(int argc, char** argv) {
    /*if (!init()) {
        return 1;
    }*/
    initt();

    // Run the main loop
    while (!quit) {
        run_game();
    }

    // Cleanup resources
    //cleanUp();

    return 0;
}
#endif
