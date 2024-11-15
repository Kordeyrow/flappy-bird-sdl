#include <SDL.h>
#include <core/game.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

Game game;
bool quit = false;

// Function to initialize the game
bool init() {
    return game.init<Playing>();
}

// Function to run the game loop
void run_game() {
    quit = game.run_until<Closed>();
#ifdef __EMSCRIPTEN__
    if (quit) {
        emscripten_cancel_main_loop(); // Stop the loop when game is done
    }
#endif
}

#ifdef __EMSCRIPTEN__
// Function to start the game loop (called from JavaScript)
extern "C" void start_game() {
    if (init()) {
        emscripten_set_main_loop(run_game, 0, 1); // Start the main loop
    }
    else {
        printf("Failed to initialize game.\n");
    }
}
#endif

#ifdef __EMSCRIPTEN__
extern "C" int main() {
    return 0; // Do nothing in main, game loop will start from start_game
}
#else
int main(int argc, char** argv) {
    if (init()) {
        while (!quit) {
            run_game();
        }
    }
    return 0;
}
#endif
