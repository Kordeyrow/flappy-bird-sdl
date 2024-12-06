#include <SDL.h>
//#include <game/game.h>
#include <engine/bird-engine.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

//Game game;
ProgramState program_state = ProgramState::RUNNING;

void setup_window() {
    int width = 460;
    int height = 640;
    double offset_x = -width * 0.04;
    double offset_y = -offset_x * 0.6;
    auto user_interface = BirdEngine::instance()->user_interface();
    auto win = user_interface->window();
    std::cout << user_interface->window()->rect().position.x << std::endl;
    //system("pause");
    Rect display_size = user_interface->window()->display_rect();
    int pos_x = display_size.size.x / 2 - width / 2 + offset_x;
    int pos_y = display_size.size.y / 2 - height / 2 + offset_y;
    Rect r{ Size{width, height}, Position{pos_x, pos_y} };
    BirdEngine::instance()->user_interface()->window()->set_rect(r);
    BirdEngine::instance()->user_interface()->renderer()->set_background_color(Color::BLUE_BIRD());
    //ImFont* score_font = gui_manager.add_font("assets/fonts/flappy-bird-score-font.ttf", 30.0f);
}

bool init() {
    /*SDL_Rect display_rect;
    SDL_GetDisplayBounds(0, &display_rect);*/
    //	
    /*_window_h = r.h * _window_h_percent_from_client;
    _window_w = _window_h * _window_w_percent_from_h;
#ifdef __EMSCRIPTEN__
        _window_h = r.h;
        _window_w = r.w;
#endif*/
//gui_manager.set_dark_colorstyle();

    // init engine
    WindowInitData win_data{ "FlappyBird" };
    UserInterfaceInitData usin_data{ win_data };
    EngineInitData eng_data{ usin_data };
    if ( ! BirdEngine::instance()->init(eng_data)) {
        return false;
    }

    setup_window();

    //return game.init<Playing>();
    return true;
}

void run_game() {
    program_state = BirdEngine::instance()->update();

    if (BirdEngine::instance()->user_interface()->input_manager()->is_key_down(SHP_K_SPACE)) {
        BirdEngine::instance()->user_interface()->renderer()->set_background_color(Color::RED());
    }
    else {
        BirdEngine::instance()->user_interface()->renderer()->set_background_color(Color::BLUE_BIRD());
    }

    //quit = game.run_until<Closed>();
#ifdef __EMSCRIPTEN__
    if (program_state == ProgramState::QUIT) {
        emscripten_cancel_main_loop(); // Stop the loop when game is done
    }
#endif
}

#ifdef __EMSCRIPTEN__
extern "C" int main() {
    return 0; // Do nothing in main, game loop will start from start_game
}
// Html StartButton callback
extern "C" void start_game() {
    if ( ! init()) {
        printf("Failed to initialize game.\n");
        return;
    } 
    emscripten_set_main_loop(run_game, 0, 1);
}
#else
int main(int argc, char** argv) {
    if ( ! init()) return 0;
    while (program_state != ProgramState::QUIT) {
        run_game();
    }
    return 0;
}
#endif
