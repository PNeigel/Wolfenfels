#include "Engine.h"

#include <windows.h>
#include <sstream>

using namespace std;

Engine::Engine()
{
	if (!Init()) {
		cout << "ERROR: Could not initialize engine." << endl;
	}
	else {
		cout << "Successfully initialized engine." << endl;
	}
	CreateShaders();

	stage = Stage(1);
}

Engine::~Engine()
{
}

bool Engine::Init()
{
	if (!glfwInit()) {
		cout << "Error: Couldn't initialize GLFW." << endl;
		return false;
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "Wolfenfels", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		cout << "Error: Couldn't create window." << endl;
		return false;
	}

	// Make the window's context current
	// Each thread can have only one current context at a time
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	if (glewInit()) {
		cout << "Error: Couldn't initialize GLEW." << endl;
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported: " << version << endl << endl;

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return true;
}

void Engine::GameLoop()
{
	Renderer renderer(stage, player);
	// Loop until the user closes the window
	double elapsed = 0;
	double delta_time = 0;
	double loop_start, render_start;
	uint32_t framecount = 0;
	while (!glfwWindowShouldClose(window)) {
		loop_start = glfwGetTime();

		UpdateKeystates();
		player.Update(delta_time, keystates);

		render_start = glfwGetTime();
		// wipe the drawing surface clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.RenderAll(stage, player, (GLuint*)&shaders[0]);

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
		// update other events like input handling 
		glfwPollEvents();
		delta_time = glfwGetTime() - render_start;
		// Calculate and display FPS
		framecount++;
		elapsed += glfwGetTime() - loop_start;
		if (elapsed > 0.5) {
			stringstream wintitle;
			wintitle << "Wolfenfels | FPS: " << framecount / elapsed;
			glfwSetWindowTitle(window, wintitle.str().c_str());
			elapsed = 0;
			framecount = 0;
		}
	}

	glfwTerminate();
}

void Engine::UpdateKeystates()
{
	keystates[Buttons::UP] = glfwGetKey(window, GLFW_KEY_UP);
	keystates[Buttons::DOWN] = glfwGetKey(window, GLFW_KEY_DOWN);
	keystates[Buttons::LEFT] = glfwGetKey(window, GLFW_KEY_LEFT);
	keystates[Buttons::RIGHT] = glfwGetKey(window, GLFW_KEY_RIGHT);
	keystates[Buttons::SHOOT] = glfwGetKey(window, GLFW_KEY_SPACE);
}

string Engine::ReadShaderGLSL(string filename)
{
	ifstream shader_f(filename);
	stringstream shader_str;
	string tmp_str;
	getline(shader_f, tmp_str);
	shader_str << tmp_str << endl;
	while (getline(shader_f, tmp_str)) {
		shader_str << tmp_str;
	}
	shader_f.close();
	return shader_str.str();
}

void Engine::CreateShaders()
{
	string projv_str = ReadShaderGLSL("Shader/projection.vert");
	const char* projection_vshader = projv_str.c_str();

	string straightv_str = ReadShaderGLSL("Shader/straight.vert");
	const char* straight_vshader = straightv_str.c_str();

	string colorf_str = ReadShaderGLSL("Shader/color.frag");
	const char* color_fshader = colorf_str.c_str();

	string texturef_str = ReadShaderGLSL("Shader/texture.frag");
	const char* texture_fshader = texturef_str.c_str();

	string texanimf_str = ReadShaderGLSL("Shader/texture_anim.frag");
	const char* texanim_fshader = texanimf_str.c_str();

	GLuint p_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(p_vs, 1, &projection_vshader, NULL);
	glCompileShader(p_vs);

	GLuint s_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(s_vs, 1, &straight_vshader, NULL);
	glCompileShader(s_vs);

	GLuint c_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(c_fs, 1, &color_fshader, NULL);
	glCompileShader(c_fs);

	GLuint t_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(t_fs, 1, &texture_fshader, NULL);
	glCompileShader(t_fs);

	GLuint ta_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ta_fs, 1, &texanim_fshader, NULL);
	glCompileShader(ta_fs);

	GLuint bgshader_program = glCreateProgram();
	glAttachShader(bgshader_program, c_fs);
	glAttachShader(bgshader_program, s_vs);
	glLinkProgram(bgshader_program);
	shaders.push_back(bgshader_program);

	GLuint texshader_program = glCreateProgram();
	glAttachShader(texshader_program, t_fs);
	glAttachShader(texshader_program, p_vs);
	glLinkProgram(texshader_program);
	shaders.push_back(texshader_program);

	GLuint player_shader = glCreateProgram();
	glAttachShader(player_shader, t_fs);
	glAttachShader(player_shader, s_vs);
	glLinkProgram(player_shader);
	shaders.push_back(player_shader);
}
