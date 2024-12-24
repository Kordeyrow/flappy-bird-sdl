#include <wing.h>
#include <res_manager/texture_manager.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

using namespace WING_API;

// runtime
ProgramState program_state = ProgramState::RUNNING;
TextureManager texture_manager;
GameObject* bird;

// window config
std::string window_title = "Flappy Bird";
Color background_color = Color::BLUE_BIRD();
int window_width = 460;
int window_height = 640;
int screen_width = Window::display_rect().size.x;
int screen_height = Window::display_rect().size.y;
double offset_x = -window_width * 0.04;
double offset_y = -offset_x * 0.6;
int window_pos_x = screen_width/2 -window_width/2 +offset_x;
int window_pos_y = screen_height/2 -window_height/2 +offset_y;

void setup_window() {
    Window::set_rect({ Position{window_pos_x, window_pos_y}, Size{window_width, window_height} });
    Window::set_title(window_title);
    // TODO: Window::set_background_color() ??
    Renderer::set_background_color(background_color);
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
    bird = Registry::create_new_gameobject();
    auto tex_data = texture_manager.get_texture_data(TEXTURE_KEY::TEXTURE_FLAPPY_BIRD_UP_WING);
    auto height_to_width_ratio = tex_data.original_size().y / tex_data.original_size().x;
    float width = 50;
    float height = width * height_to_width_ratio;
    bird->get_component<Transform>()->rect() = Rect{ 
        Position { window_width / 2 - width / 2, window_width / 2 - height/2}, 
        Size { width, height } 
    };
    bird->add_component<Sprite>(bird->get_component<Transform>(), texture_manager.get_texture_id(TEXTURE_KEY::TEXTURE_FLAPPY_BIRD_UP_WING));
    bird->add_component<Rigidbody>(bird->get_component<Transform>());
    Registry::add_gameobject_to_scene(bird, main_scene_id);


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

    // jump
    if (InputManager::is_key_just_pressed(WING_K_SPACE)
        || (Window::CheckFOVState(bird->get_component<Transform>()->rect()) == FOVState::PARTIAL
            && Window::CheckClosestFOVBound(bird->get_component<Transform>()->rect()) == FOVBounds::BOTTOM)) {
        Renderer::set_background_color(Color::RED());
        bird->get_component<Rigidbody>()->set_velocity(0, -800);
    }
    else {
        Renderer::set_background_color(Color::BLUE_BIRD());
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
