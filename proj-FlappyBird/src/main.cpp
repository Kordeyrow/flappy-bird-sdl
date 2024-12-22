//#include <SDL.h>
#include <wing.h>
//#include <game/game.h> 
#include <res_manager/texture_manager.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

using namespace WING_API;

// runtime
//Game game;
ProgramState program_state = ProgramState::RUNNING;
TextureManager texture_manager;
GameObject* pipe;

void setup_window() {
    int width = 460;
    int height = 640;
    double offset_x = -width * 0.04;
    double offset_y = -offset_x * 0.6;
    Rect display_size = Window::display_rect();
    int pos_x = display_size.size.x / 2 - width / 2 + offset_x;
    int pos_y = display_size.size.y / 2 - height / 2 + offset_y;
    Rect r{ Position{pos_x, pos_y}, Size{width, height} };
    Window::set_rect(r);
    Window::set_title("Flappy Bird");
    Renderer::set_background_color(Color::BLUE_BIRD());
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

    setup_window();

    texture_manager.init();

    auto main_scene_id = Registry::create_new_scene();
    Registry::set_start_scene(main_scene_id);
    pipe = Registry::create_new_gameobject();
    pipe->get_component<Transform>()->set(100, 100, 100, 100);
    pipe->add_component<Sprite>(pipe->get_component<Transform>(), texture_manager.get_texture_id(TEXTURE_KEY::TEXTURE_FLAPPY_BIRD_UP_WING));
    pipe->add_component<Rigidbody>(pipe->get_component<Transform>());
    Registry::add_gameobject_to_scene(pipe, main_scene_id);


    //ImFont* score_font = gui_manager.add_font("assets/fonts/flappy-bird-score-font.ttf", 30.0f);
    //return game.init<Playing>();
    return true;
}

void run_game() {
    //////////////program_state = BirdEngine::instance()->update();

    // Move
    //auto p = pipe->get_component<Transform>()->position;
    //pipe->get_component<Transform>()->set(p.x + 1, 100, 100, 100);

    Wing::run_game();
    // TEST
    // ======================
    //
    if (InputManager::is_key_down(WING_K_SPACE)) {
        Renderer::set_background_color(Color::RED());
    }
    else {
        Renderer::set_background_color(Color::BLUE_BIRD());
    }
    // ======================

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
