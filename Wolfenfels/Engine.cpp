#include "Engine.h"

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
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return true;
}

void Engine::GameLoop()
{
	// Loop until the user closes the window
	double elapsed = 0;
	uint32_t framecount = 0;
	while (!glfwWindowShouldClose(window)) {
		double start_time = glfwGetTime();

		UpdateKeystates();
		player.Update(keystates);

		// wipe the drawing surface clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		GLuint MatrixID = glGetUniformLocation(shader_program, "MVP");

		//ComputeView();
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &player.mvp[0][0]);
		stage.RenderStage(bgshader_program, shader_program);

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
		// update other events like input handling 
		glfwPollEvents();

		// Calculate and display FPS
		framecount++;
		elapsed += glfwGetTime() - start_time;
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
}

void Engine::CreateShaders()
{
	// Shaders in GLSL
	const char* vertex_shader =
		"#version 400\n" // Version of GLSL
		"layout(location = 0) in vec3 v_pos;"
		"layout(location = 1) in vec3 v_col;"
		"uniform mat4 MVP;"
		"out vec3 color;"
		"void main() {"
		"  color = v_col;"
		"  gl_Position = MVP * vec4(v_pos, 1.0);"
		"}";

	const char* bg_shader =
		"#version 400\n" // Version of GLSL
		"layout(location = 0) in vec3 v_pos;"
		"layout(location = 1) in vec3 v_col;"
		"out vec3 color;"
		"void main() {"
		"  color = v_col;"
		"  gl_Position = vec4(v_pos, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 400\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main() {"
		"  frag_color = vec4(color, 1.0);"
		"}";

	// Compile and link shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	GLuint bgs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(bgs, 1, &bg_shader, NULL);
	glCompileShader(bgs);

	shader_program = glCreateProgram();

	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);

	bgshader_program = glCreateProgram();
	glAttachShader(bgshader_program, fs);
	glAttachShader(bgshader_program, bgs);
	glLinkProgram(bgshader_program);
}
