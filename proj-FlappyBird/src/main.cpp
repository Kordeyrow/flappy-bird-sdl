#include <SDL.h>
#include <WING.h>
//#include <game/game.h> 
#include <res_manager/texture_manager.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

// runtime
//Game game;
ProgramState program_state = ProgramState::RUNNING;

//std::shared_ptr<WindowAPI> window;
////////std::shared_ptr<Renderer> renderer;
////////std::shared_ptr<InputManager> input_manager;
////////std::shared_ptr<AssetManager> asset_manager;
TextureManager* texture_manager;
//////
void setup_window() {
    int width = 460;
    int height = 640;
    double offset_x = -width * 0.04;
    double offset_y = -offset_x * 0.6;
    //auto device_interface = BirdEngine::instance()->device_interface();
    //auto win = device_interface->window();
    std::cout << Window::rect().position.x << std::endl;
    //system("pause");
    Rect display_size = Window::display_rect();
    int pos_x = display_size.size.x / 2 - width / 2 + offset_x;
    int pos_y = display_size.size.y / 2 - height / 2 + offset_y;
    Rect r{ Position{pos_x, pos_y}, Size{width, height} };
    Window::set_rect(r);
    Renderer::set_background_color(Color::BLUE_BIRD());
}

bool init_engine() {
    /////////*WindowInitData win_data{ "FlappyBird" };
    ////////UserInterfaceInitData usin_data{ win_data };
    ////////EngineInitData eng_data{ usin_data };
    ////////if (!BirdEngine::instance()->init()) {
    ////////    return false;
    ////////}*/
    return true;
}

bool init_game() {
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



    /////////*auto device_interface = BirdEngine::instance()->device_interface();
    ////////window = device_interface->window();
    ////////renderer = device_interface->renderer();
    ////////input_manager = device_interface->input_manager();
    ////////asset_manager = device_interface->asset_manager();

    setup_window();

    /*texture_manager = new TextureManager(asset_manager);
    texture_manager->init();*/


    // TEST
    // ======================
    //
    /*device_interface->renderer()->add_drawwable (
        Drawable { 
            texture_manager->get_texture_id(TEXTURE_KEY::PIPE),
            Transform { Position{100, 100}, Size{140, 140}, 0 } 
        }
    );*/

    //BIRDENGINE_API::Window::set_rect({});
    //auto main_scene_id = Registry::instance()->create_new_scene();
    //auto pipe = Registry::instance()->create_new_gameobject();
    ///*pipe->add_component<Sprite>(texture_manager->get_texture_id(TEXTURE_KEY::PIPE));
    //Registry::instance()->add_gameobject_to_scene(pipe, main_scene_id);*/
    //Registry::instance()->set_starting_scene(main_scene_id);
    // ======================
    

    //ImFont* score_font = gui_manager.add_font("assets/fonts/flappy-bird-score-font.ttf", 30.0f);
    //return game.init<Playing>();
    return true;
}

bool init() {
    if ( ! init_engine()) {
        return false;
    }
    if ( ! init_game()) {
        return false;
    }
    return true;
}

void run_game() {
    //////////////program_state = BirdEngine::instance()->update();

    Wing::run_game();
    // TEST
    // ======================
    //
    /////////////////////*if (input_manager->is_key_down(SHP_K_SPACE)) {
    ////////////////////    renderer->set_background_color(Color::RED());
    ////////////////////}
    ////////////////////else {
    ////////////////////    renderer->set_background_color(Color::BLUE_BIRD());
    ////////////////////}*/
    // ======================

    //Renderer::set_background_color(Color::BLUE_BIRD());

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
