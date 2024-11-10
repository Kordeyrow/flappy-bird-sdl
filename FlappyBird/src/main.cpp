#include <SDL.h>
#include <core/game.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

Game game;
bool quit = false;

bool init() {
    return game.init<Playing>();
}

void run_game() {
    quit = game.run_until<Closed>();
    if (quit) {
        exit(0);
    }
}

#ifdef __EMSCRIPTEN__
int main() {
    if ( ! init()) {
        return 1;
    }
    emscripten_set_main_loop(run_game, 0, 1);
    return 0;
}
#else
int main(int argc, char** argv) {
    if ( ! init()) {
        return 1;
    }
    while (!quit) {
        run_game();
    }
    return 0;
}
#endif
