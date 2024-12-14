#pragma once
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include <functional>
#include <chrono>
#include <map>
#include <typeindex>
#include <string>

#include <sdl2imgui/imgui.h>
#include <sdl2imgui/imgui_impl_sdl2.h>
#include <sdl2imgui/imgui_impl_sdlrenderer2.h>

#include <SDL.h>
#include <SDL_image.h>

#include <entities/flappy_bird.h>
#include <res_manager/texture_manager.h>
#include <system_component/updatable.h>
#include <entities/pipe.h>
#include <state_machine_base/state_machine.h>
#include "game_state.h"
#include <queue>

#include <GL/glew.h>
#include <SDL_opengl.h>


class Game {
public:
	// SDL
	SDL_Window* window() { return _window; }
	SDL_Renderer* renderer() { return _renderer; }
	// Managers 
	TextureManager* texture_manager() { return _texture_manager; }
	// Gameobjects
	std::vector<Drawable*>* sprites() { return &_sprites; }
	std::vector<Updatable*>* updatables() { return &_updatables; }
	// Window
	double window_h() const { return _window_h; }
	double window_w() const { return _window_w; }
	// Player
	FlappyBird* player() { return _player; }
	int score() const { return _score; }
	void add_score() {_score++; }
	// Pipe
	double pipe_size_x() const { return _pipe_size_x; }
	// 
	bool is_debug_gizmos_on() const { return _is_debug_gizmos_on; }
	void toggle_debug_gizmos() { _is_debug_gizmos_on = !_is_debug_gizmos_on; }
	void set_debug_gizmos(bool val) { _is_debug_gizmos_on = val; }

	StateMachine* state_machine() { return _state_machine; }
	std::tuple<Pipe*, Pipe*> next_pipes() { return _next_pipes; }

	GameState* start_state;

private:
	//-- Config
	// Window
	const double _window_h_percent_from_client = 0.7;
	const double _window_w_percent_from_h = 0.75;

	//-- Runtime
	// Window
	double _window_h;
	double _window_w;
	// SDL
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	// OpenGL
	SDL_GLContext gl_context;
	GLuint shader_program;
	GLuint VBO, VAO;


	const char* vertex_shader_source = R"(
#version 330 core
layout (location = 0) in vec3 aPos;    // Vertex position
layout (location = 1) in vec2 aTexCoord; // Texture coordinates

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
	//TexCoord = vec2(1, 1);
}
)";

	const char* fragment_shader_source = R"(
#version 330 core
layout (location = 0) out vec4 color;

in vec2 TexCoord;

uniform sampler2D tex; // Texture sampler

void main() {
    vec4 FragColor = texture(tex, TexCoord);
	// color = vec3(FragColor.x, FragColor.y, 1);
	// color = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0);
	color = FragColor;
}
)";

//	const char* fragment_shader_source = R"(
//#version 330 core
//out vec4 FragColor;
//
//void main() {
//    FragColor = vec4(1.0, 1.0, 0.0, 1.0); // Output red
//}
//)";
	// Managers 
	TextureManager* _texture_manager;
	// Gameobjects
	std::vector<Drawable*> _sprites;
	std::vector<Updatable*> _updatables;
	// Player
	FlappyBird* _player;
	// Pipe
	double _pipe_size_x = 64;
	// StateMachine
	StateMachine* _state_machine{};
	// score
	ImFont* score_font = nullptr;
	int _score = 0;
	// pipe
	std::queue<std::tuple<Pipe*, Pipe*>> pipe_tuple_queue; // when "_next_pipes" is passed by player, get next
	std::tuple<Pipe*, Pipe*> _next_pipes;
	// debug
	bool _is_debug_gizmos_on = false;

public:
	Game() {}
		

	void CheckOpenGLError(const char* stmt, const char* fname, int line)
	{
		bool found_error = false;
		GLenum err = glGetError();
		while (err) {
			if (err == GL_NO_ERROR) break;
			printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
			found_error = true;
			err = glGetError();
		}
		if (found_error) {
			abort();
		}
	}

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif


	template<typename StartStateType>
	bool init() {

		// SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
			system("pause");
			return false;
		}

		// IMG
		if (IMG_Init(IMG_INIT_PNG) < 0) {
			std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
			return false;
		}

		try
		{
			_window = SDL_CreateWindow(
				"Flappy Bird",                // Window title
				0,       // X position
				0,       // Y position
				0,                          // Width
				0,                          // Height
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN         // Borderless flag
			);

			if (!_window) {
				std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
				SDL_Quit();
				return -1;
			}


			// Create a renderer
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
			if (!_renderer) {
				std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
				SDL_DestroyWindow(_window);
				SDL_Quit();
				return -1;
			}

			SDL_Rect r;
			SDL_GetDisplayBounds(0, &r);

			/*_window_h = r.h * _window_h_percent_from_client;
			_window_w = _window_h * _window_w_percent_from_h;*/
			#ifdef __EMSCRIPTEN__
			_window_h = r.h;
			_window_w = r.w;
			#endif
			_window_w = 460;
			_window_h = 640;

			SDL_SetWindowSize(_window, _window_w, _window_h);
			double offset_x = -r.w * 0.04;
			double offset_y = -offset_x * 0.6;
			SDL_SetWindowPosition(_window, r.w / 2 - _window_w / 2 + offset_x, r.h / 2 - _window_h / 2 + offset_y);

			SDL_SetWindowTitle(_window, "Flappy Bird");

			init_imgui();
		}
		catch (const std::exception& e)
		{
			std::cout << "Error initializing: " << e.what() << std::endl;
			return false;
		}

		//// Request OpenGL 3.3 Core context
		//GL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//int major, minor, profile;
		//SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
		//SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
		//SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);
		//std::cout << "OpenGL version: " << major << "." << minor << " Profile: " << profile << std::endl;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Create OpenGL context
		//GL_CHECK(gl_context = SDL_GL_CreateContext(_window));
		gl_context = SDL_GL_CreateContext(_window);
		if (!gl_context) {
			std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
			return false;
		}

		// Initialize GLEW or equivalent
		if (glewInit() != GLEW_OK) {
			std::cerr << "Failed to initialize GLEW" << std::endl;
			return false;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// opengl view port
		SDL_GL_MakeCurrent(_window, gl_context);
		glViewport(0, 0, _window_w, _window_h);

		glClearColor(2.0f, 0.5f, 1.0f, 1.0f); // Background color
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		//GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		// Check for vertex shader compilation errors
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		// Check for fragment shader compilation errors
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);

		// Check for shader program linking errors
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex_shader);
		//GL_CHECK(glDeleteShader(fragment_shader));
		glDeleteShader(fragment_shader);

		/*float vertices[] = {
		0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);*/


		// texture
		_texture_manager = new TextureManager();
		_texture_manager->load_init_textures();

		// state machine
		_state_machine = new StateMachine();
		_state_machine->init(CreateState<StartStateType>());


		return true;
	}

	//void init() {
	//	// SDL
	//	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
	//		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
	//		system("pause");
	//		return;
	//	}

	//	// IMG
	//	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {
	//		std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
	//		return;
	//	}

	//	// window
	//	_window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	//	if (!_window) {
	//		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
	//		system("pause");
	//		return;
	//	}

	//	// renderer
	//	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	//	if (!_renderer) {
	//		std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
	//		return;
	//	}

	//	init_imgui();

	//	// window info
	//	_window_w = (double)SDL_GetWindowSurface(_window)->w;
	//	_window_h = (double)SDL_GetWindowSurface(_window)->h;

	//	// texture
	//	_texture_manager = new TextureManager(_renderer);
	//	_texture_manager->load_init_textures();

	//	// state machine
	//	_state_machine = new StateMachine();
	//	_state_machine->init(CreateState<StartStateType>());
	//}

	void init_imgui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		// Load custom font (make sure the .ttf file path is correct)
		score_font = io.Fonts->AddFontFromFileTTF("assets/fonts/flappy-bird-score-font.ttf", 30.0f);
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
		ImGui_ImplSDLRenderer2_Init(_renderer);

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
	}

public:
	void render_score(const char* score, ImFont* font = nullptr) {
		if (font == nullptr) { font = score_font; }

		// Load custom font (make sure the .ttf file path is correct)
		ImGuiStyle& style = ImGui::GetStyle();

		// Set all borders and window decorations to zero or transparent
		style.WindowBorderSize = 0.0f; // No border on windows
		style.FrameBorderSize = 0.0f;  // No border on frames
		style.WindowRounding = 0.0f;   // No corner rounding for window
		style.FrameRounding = 0.0f;    // No corner rounding for frames

		//// Make window and frame backgrounds fully transparent
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushFont(font);
		//auto prev_cursor_pos_x = ImGui::GetCursorPosX();
		auto textWidth = ImGui::CalcTextSize(score).x;
		const char* score_0 = &score[0];
		auto textWidth_fix_offset = ImGui::CalcTextSize(score_0).x;
		ImGui::SetCursorPosX(_window_w / 2 - textWidth / 2 + textWidth_fix_offset/2);
		ImGui::Text(score);
		ImGui::PopFont();
		ImGui::PopStyleColor();
		//ImGui::SetCursorPosX(prev_cursor_pos_x);
	}

	template<typename EndStateType>
	bool run_until() {
		State* curr = _state_machine->get_current_state();
		EndStateType* s = dynamic_cast<EndStateType*>(curr);
		if (s) {
			return true;
		}
		_state_machine->run();
		return false;
	}

	/*void spawn_player() {
		Vector2 player_size = Vector2{ 48, 36 };
		Vector2 player_start_pos = Vector2{ _window_w / 2, _window_h / 2 };
		double ground_y = _window_h + 3;
		_player = new FlappyBird{
			_texture_manager->get_texture(TextureManager::TEXTURE_FLAPPY_BIRD_UP_WING),
			ground_y,
			player_start_pos,
			player_size,
			17
		};
		_sprites.push_back(_player);
		_updatables.push_back(_player);
	}*/
	void spawn_player() {
		Vector2 player_size = Vector2{ 48, 36 };
		Vector2 player_start_pos = Vector2{ _window_w / 2, _window_h / 2 };
		double ground_y = _window_h + 3;

		GLuint player_texture = _texture_manager->get_texture(TextureManager::TEXTURE_FLAPPY_BIRD_UP_WING);
		if (player_texture == 0) {
			std::cerr << "Error: Failed to load player texture!" << std::endl;
			return;
		}

		_player = new FlappyBird{
			player_texture, // Use OpenGL texture ID
			ground_y,
			player_start_pos,
			player_size,
			17
		};
		_sprites.push_back(_player);
		_updatables.push_back(_player);
	}

	void spawn_pipe(double window_w, double window_h, float speed_x) {
		double size_y = 440;
		double pair_gap_size = 156;

		// random y
		double max_y_offset = 128;
		double random_y_offset = max_y_offset * generate_biased_random();

		Vector2 pipe_size = Vector2{ pipe_size_x(), size_y };
		double x_offset = pipe_size.x / 2;
		double y_offset = random_y_offset;
		Vector2 pipe_start_pos = Vector2{ window_w + x_offset, window_h /2 + y_offset };

		Pipe* bottom_pipe = new Pipe{
			_state_machine,
			_texture_manager->get_texture(TextureManager::PIPE),
			pipe_start_pos + Vector2{0, +size_y/2 +pair_gap_size / 2},
			pipe_size,
			speed_x
		};
		_sprites.push_back(bottom_pipe);
		_updatables.push_back(bottom_pipe);

		Pipe* top_pipe = new Pipe{
			_state_machine,
			_texture_manager->get_texture(TextureManager::PIPE),
			pipe_start_pos + Vector2{0, -size_y/2 -pair_gap_size /2},
			pipe_size,
			speed_x,
			SDL_FLIP_VERTICAL
		};
		_sprites.push_back(top_pipe);
		_updatables.push_back(top_pipe);

		pipe_tuple_queue.push({ bottom_pipe, top_pipe });
		if ( ! std::get<0>(_next_pipes)) {
			update_next_pipes();
		}
	}
	
	double generate_biased_random(double bias_strength = 3.8, double sign_flip_chance = 0.52) {
		static int last_sign = 1; // 1 for positive, -1 for negative

		// Generate a random float in the range [0, 1]
		double raw_value = (rand() % 101) / 100.0f;

		// Apply the bias towards the extremes using a power function
		float biased_value = pow(raw_value, 1.0f / (bias_strength / 10));

		// Map the biased value to the range (-0.5, 0.5)
		biased_value = (biased_value) * last_sign;

		// Decide whether to flip the sign based on sign_flip_chance
		if ((rand() % 100) < (sign_flip_chance * 100)) {
			last_sign = -last_sign;  // Flip the sign
		}

		return biased_value;
	}

	void update_next_pipes() {
		_next_pipes = std::move(pipe_tuple_queue.front());
		pipe_tuple_queue.pop();
	}

	uint32_t get_current_time()
	{
		return SDL_GetTicks64();
	}

	// TODO: apply on vector_changed
	static void sort_sprites_by_layer(std::vector<Drawable*>& sprites) {
		std::sort(sprites.begin(), sprites.end(), [](Drawable* a, Drawable* b) {
			return a->layer_index() > b->layer_index();
			});
	}

	void draw_sprites()
	{
		sort_sprites_by_layer(_sprites);

		/*for (auto s : _sprites) {
			auto rect = s->get_rect();
			SDL_RenderCopyEx(_renderer, s->get_texture(), NULL, &rect, s->get_rotation(), NULL, s->flip());
		}*/

		for (Drawable* s : _sprites) {
			// Get sprite properties
			SDL_Rect rect = s->get_rect();
			GLuint texture = s->get_texture(); // Ensure your Drawable class provides OpenGL texture ID
			float rotation = s->get_rotation();
			SDL_RendererFlip flip = s->flip();

			// Bind the texture
			glBindTexture(GL_TEXTURE_2D, texture);

			// Render quad with the sprite's texture
			render_quad(texture, rect, rotation, flip);

			// Unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void render_quad(GLuint texture, const SDL_Rect& rect, float rotation, SDL_RendererFlip flip) {

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		
		// Use the shader program
		//GL_CHECK(glUseProgram(shader_program));
		glUseProgram(shader_program);

		// Activate texture unit 0
		glActiveTexture(GL_TEXTURE0);
		// Bind the texture to the active texture unit
		glBindTexture(GL_TEXTURE_2D, texture);

		// Set the sampler uniform to use texture unit 0
		GLint textureLocation = glGetUniformLocation(texture, "tex");
		glUniform1i(textureLocation, 0);

		// Calculate NDC coordinates
		float x1 = 2.0f * rect.x / _window_w - 1.0f;
		float y1 = 1.0f - 2.0f * rect.y / _window_h;
		float x2 = 2.0f * (rect.x + rect.w) / _window_w - 1.0f;
		float y2 = 1.0f - 2.0f * (rect.y + rect.h) / _window_h;

		// Quad vertices: position (x, y, z), texture (u, v)
		float vertices[] = {
			x1, y1, 0.0f, 0.0f, flip == SDL_FLIP_VERTICAL ? 1.0f : 0.0f,
			x2, y1, 0.0f, 1.0f, flip == SDL_FLIP_VERTICAL ? 1.0f : 0.0f,
			x2, y2, 0.0f, 1.0f, flip == SDL_FLIP_VERTICAL ? 0.0f : 1.0f,
			x1, y2, 0.0f, 0.0f, flip == SDL_FLIP_VERTICAL ? 0.0f : 1.0f
		};

		GLuint indices[] = { 0, 1, 2, 2, 3, 0 };

		GLuint VBO, VAO, EBO;

		// Generate and bind the VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// EBO
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Texture coordinate attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/


		// Draw the quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//GL_CHECK();


		/*int X = 0;
		int Y = 0;
		int Width = 100;
		int Height = 100;

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(X, Y, 0);
		glTexCoord2f(1, 0); glVertex3f(X + Width, Y, 0);
		glTexCoord2f(1, 1); glVertex3f(X + Width, Y + Height, 0);
		glTexCoord2f(0, 1); glVertex3f(X, Y + Height, 0);
		glEnd();*/


		// Cleanup
		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);



		//GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		glBindTexture(GL_TEXTURE_2D, 0);
	}




	void draw_background()
	{
		//// Clear the window to white
		//SDL_SetRenderDrawColor(_renderer, 120, 200, 250, 255);
		//int w, h;
		//SDL_GetWindowSize(_window, &w, &h);
		//SDL_Rect r{ 0, 0, w, h };
		//SDL_RenderFillRect(_renderer, &r);
		//SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);


		glClearColor(0.47f, 0.78f, 0.98f, 1.0f); // Sky blue background (RGB: 120, 200, 250)
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void render_ui() {
		//ImGui::NewFrame();

		//// Custom window with no title bar or resizing options
		//ImGuiCond c = ImGuiCond_FirstUseEver;

		//// update imgui.ini file on start
		//static bool win_init = false;
		//if ( ! win_init) { c = ImGuiCond_Once; win_init = true;}

		//ImGui::SetNextWindowSize(ImVec2(_window_w, 0), c);
		//ImGui::SetNextWindowPos(ImVec2(0, _window_h / 30), c);
		//ImGui::Begin("ScoreWindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		//
		//render_score(std::to_string(_score).c_str());

		//ImGui::End();
		//ImGui::Render();
		//ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), _renderer);


		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplSDL2_NewFrame(_window);
		//ImGui::NewFrame();

		//// Create the score window
		//ImGui::Begin("Score", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
		//render_score(std::to_string(_score).c_str());
		//ImGui::End();

		//// Render ImGui
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void draw_circle(SDL_Renderer* renderer, const CircleCollider& circle, SDL_Color color = { 255, 0, 0, 255 }, bool outline_only = true, int outline_thickness = 4) {
		// Ensure renderer is valid
		if (!renderer) return;

		SDL_Color red = { 255, 0, 0, 255 };
		SDL_Color black = { 0, 0, 0, 60 };

		int centerX = circle.transform->position.x;
		int centerY = circle.transform->position.y;
		int radius = circle.radius();

		if (outline_only) {
			// Outline circle with adjustable thickness
			for (int t = 0; t < outline_thickness; ++t) {

				if (t == 0 || t == outline_thickness-1) {
					SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
				}
				else {
					SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
				}

				int adjusted_radius = radius - t;
				int x = adjusted_radius;
				int y = 0;
				int decisionOver2 = 1 - x;   // Decision criterion divided by 2 (for better precision)

				while (y <= x) {
					// Draw the eight octants of the circle
					SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
					SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
					SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
					SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
					SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
					SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
					SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
					SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);

					y++;
					if (decisionOver2 <= 0) {
						decisionOver2 += 2 * y + 1; // Change decision criterion for next y increment
					}
					else {
						x--;
						decisionOver2 += 2 * (y - x) + 1; // Change decision criterion for next y and x decrement
					}
				}
			}
		}
		else {
			// Filled circle using the original Midpoint Circle Algorithm
			for (int w = 0; w < radius * 2; w++) {
				for (int h = 0; h < radius * 2; h++) {
					int dx = radius - w; // horizontal offset
					int dy = radius - h; // vertical offset
					if ((dx * dx + dy * dy) <= (radius * radius)) {
						SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
					}
				}
			}
		}
	}

	void draw_rectangle(SDL_Renderer* renderer, const RectangleCollider& rect, SDL_Color color = { 255, 0, 0, 255 }, bool outline_only = true, int outline_thickness = 4) {
		// Ensure renderer is valid
		if (!renderer) return;

		SDL_Color red = { 255, 0, 0, 255 };
		SDL_Color black = { 0, 0, 0, 60 };

		// Calculate the rectangle's corners
		double left = rect.left();
		double right = rect.right();
		double top = rect.top();
		double bottom = rect.bottom();

		if (outline_only) {
			// Draw outline with adjustable thickness
			for (int t = 0; t < outline_thickness; ++t) {
				if (t == 0 || t == outline_thickness - 1) {
					SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
				}
				else {
					SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
				}
				// Adjust the position of each outline layer
				SDL_Rect outlineRect = {
					(int)(left + t),        // Left
					(int)(top + t),         // Top
					(int)(right - left - 2 * t),  // Width (decreasing as t increases)
					(int)(bottom - top - 2 * t)  // Height (decreasing as t increases)
				};
				SDL_RenderDrawRect(renderer, &outlineRect);
			}
		}
		else {
			// Draw filled rectangle
			SDL_Rect fillRect = { (int)left, (int)top, (int)(right - left), (int)(bottom - top) };
			SDL_RenderFillRect(renderer, &fillRect);
		}
	}

	void draw_debug_info() {
		// Example: Draw a red rectangle for player collision bounds
		//glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // Red color
		//glBegin(GL_LINE_LOOP);
		//glVertex2f(player->left(), player->top());
		//glVertex2f(player->right(), player->top());
		//glVertex2f(player->right(), player->bottom());
		//glVertex2f(player->left(), player->bottom());
		//glEnd();

		draw_circle(_renderer, *_player->circle_collider());
		for (Drawable* s : _sprites) {
			RectangleCollider* col = dynamic_cast<RectangleCollider*>(s);
			if (col) {
				draw_rectangle(_renderer, *col);
			}
		}
	}



	void render() {
		/*draw_backgroung();
		draw_sprites();
		render_ui();
		if (_is_debug_gizmos_on) draw_debug_info();
		SDL_RenderPresent(_renderer);
		SDL_RenderClear(_renderer);*/
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. Draw the background
		draw_background();

		// 2. Draw the sprites (pipes, player, etc.)
		draw_sprites();

		// 3. Render the ImGui-based UI
		render_ui();

		// 4. Draw debug gizmos if enabled
		if (_is_debug_gizmos_on) {
			draw_debug_info();
		}

		// Swap the OpenGL buffers
		SDL_GL_SwapWindow(_window);
	}

	void reset() {
		// clear_heap
		while ( ! pipe_tuple_queue.empty())
		{ 
			auto& t = pipe_tuple_queue.front();
			Pipe* p1 = std::get<0>(t);
			Pipe* p2 = std::get<1>(t);
			if (p1) { delete p1; }
			if (p2) { delete p2; }
			pipe_tuple_queue.pop();
		}

		Pipe* p1 = std::get<0>(_next_pipes);
		Pipe* p2 = std::get<1>(_next_pipes);
		if (p1) { delete p1; }
		if (p2) { delete p2; }
		_next_pipes = { nullptr, nullptr };

		if (_player) { delete _player; }

		/*for (auto p : _sprites)
		{
			delete p;
		}*/
		_sprites.clear();

		/*for (size_t i = 0; i < _updatables.size(); i++) 
		{ 
			if (_updatables[i]) 
				delete _updatables[i]; 
		}*/
		_updatables.clear();

		// reset vals
   		_score = 0;
	}

	void close() {
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyWindow(_window);
		_window = nullptr;

		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;

		_texture_manager->kill();
		_texture_manager = nullptr;

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shader_program);
		SDL_GL_DeleteContext(gl_context);

		IMG_Quit(); 
		SDL_Quit();
	}

	template<typename GameStateType>
	GameState* CreateState() {
		return (GameState*) new GameStateType(this, _state_machine);
	}
};

class Menu : public GameState {
public:
	Menu(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {

	};

	State* run() override {
		return this;
	};

	void exit() override {

	};
};

class Closed : public GameState {
public:
	Closed(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {

	};

	State* run() override {
		return this;
	};

	void exit() override {

	};
};

class Closing : public GameState {
public:
	Closing(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {
		game->close();
	};

	State* run() override {
		return game->CreateState<Closed>();
	};

	void exit() override {

	};
};

class GameOver;

class Playing : public GameState, public GameplayBase {
private:
	//// CONFIG
	const float speed_x = -130;
	const double spawn_gap = 115;
	double real_spawn_gap_seconds = spawn_gap / abs(speed_x) + game->pipe_size_x() / abs(speed_x);
	//// STATE
	// spawn pipe
	double current_spawn_timer_seconds = 0;
	// game loop
	bool quit = false;
	double total_elapsed_time = 0;
	uint32_t previous_time = 0;
	int score = 0;
public:
	Playing(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {
		game->reset();

		// spawn pipe
		current_spawn_timer_seconds = 0;
		game->spawn_pipe(game->window_w(), game->window_h(), speed_x);

		// player
		game->spawn_player();
		game->player()->jump();

		// game loop
		total_elapsed_time = 0;
		previous_time = game->get_current_time();
	};

	State* run() override {
		// time
		auto current_time = game->get_current_time();
		auto elapsed_time_seconds = (current_time - previous_time) / 1000.0f; // Convert to seconds.
		previous_time = current_time;
		
		// input
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			SDL_Keycode keycode = e.key.keysym.sym;
			switch (e.type) {
			case SDL_QUIT:
				return game->CreateState<Closing>();
			case SDL_KEYDOWN:
				if (keycode == SDLK_SPACE) {
					if (game->player()->dead() == false) {
						game->player()->jump();
					}
					if (game->player()->fallen()) {
						// reset
					}
				}
				else if (keycode == SDLK_b) {
					game->toggle_debug_gizmos();
				}
				break;
			case SDL_KEYUP:
				break;
			}
		}

		// update
		for (auto u : *game->updatables()) {
			u->update(elapsed_time_seconds);
		}

		// spawn pipes
		current_spawn_timer_seconds += elapsed_time_seconds;
		if (current_spawn_timer_seconds >= real_spawn_gap_seconds) {
			current_spawn_timer_seconds = 0;
			game->spawn_pipe(game->window_w(), game->window_h(), speed_x);
		}

		bool player_dead = false;

		// physics
		total_elapsed_time += elapsed_time_seconds;
		if (total_elapsed_time > 0.02) {
			total_elapsed_time -= 0.02;

			//// get colliders
			//std::vector<Collider*> cols;
			//for (auto u : *game->updatables()) {
			//	Collider* col = dynamic_cast<Collider*>(u);
			//	if (col) {
			//		cols.push_back(col);
			//	}
			//}

			//// check collision
			//for (size_t i = 0; i < cols.size(); i++)
			//{
			//	for (size_t j = i + 1; j < cols.size(); j++)
			//	{
			//		if (game->collision_manager().is_colliding(cols[i], cols[j])) {
			//			cols[i]->collided(cols[j]);
			//			cols[j]->collided(cols[i]);
			//		}
			//	}
			//}

			Pipe* bottom_pipe = std::get<0>(game->next_pipes());
			if (bottom_pipe) {

				// update next_pipes (score)
				double r = bottom_pipe->right();
				double l = game->player()->left();
  				if (bottom_pipe->right() < game->player()->left()) {
					game->update_next_pipes();
					bottom_pipe = std::get<0>(game->next_pipes());
					game->add_score();
				}

				// kill player
				Pipe* top_pipe = std::get<1>(game->next_pipes());
				bool colliding = CollisionChecker::is_colliding(*game->player()->circle_collider(), *bottom_pipe->rectangle_collider());
				if ( ! colliding) {
					colliding = CollisionChecker::is_colliding(*game->player()->circle_collider(), *top_pipe->rectangle_collider());
				}
				if (colliding) {
					game->player()->kill();
				}
			}
		}

		//reder
		game->render();

		if (game->player()->dead()) {
			return game->CreateState<GameOver>();
		}
		return this;
	};

	void exit() override {
	};
};

class GameOver : public GameState {
public:
	GameOver(Game* game, StateMachineEventEmitter* emitter) : GameState{ game, emitter } {}

	void enter() override {

	};

	State* run() override {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			SDL_Keycode keycode = e.key.keysym.sym;
			switch (e.type) {
				case SDL_QUIT:
					return game->CreateState<Closing>();
				case SDL_KEYDOWN:
					if (keycode == SDLK_SPACE) {
						return game->CreateState<Playing>();
					}
					break;
			}
		}
		return this;
	};

	void exit() override {

	};
};