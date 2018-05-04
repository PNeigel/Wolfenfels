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
	uint32_t framecount = 0;
	while (!glfwWindowShouldClose(window)) {
		double start_time = glfwGetTime();

		UpdateKeystates();
		player.Update(keystates);

		// wipe the drawing surface clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(shaders[1]);
		GLuint MatrixID = glGetUniformLocation(shaders[1], "MVP");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &player.mvp[0][0]);

		renderer.RenderAll(stage, player, (GLuint*)&shaders[0]);

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

	const char* projection_vshader =
		"#version 400\n" // Version of GLSL
		"layout(location = 0) in vec3 pos;"
		"layout(location = 1) in vec2 UV_in;"
		"layout(location = 2) in vec3 color_in;"
		"uniform mat4 MVP;"
		"out VertexData{"
		"  vec2 UV;"
		"  vec3 color;"
		"} outp;"
		"void main() {"
		"  outp.UV = UV_in;"
		"  outp.color = color_in;"
		"  gl_Position = MVP * vec4(pos, 1.0);"
		"}";

	const char* straight_vshader =
		"#version 400\n" // Version of GLSL
		"layout(location = 0) in vec3 pos;"
		"layout(location = 1) in vec2 UV_in;"
		"layout(location = 2) in vec3 color_in;"
		"out VertexData{"
		"  vec2 UV;"
		"  vec3 color;"
		"} outp;"
		"void main() {"
		"  outp.UV = UV_in;"
		"  outp.color = color_in;"
		"  gl_Position = vec4(pos, 1.0);"
		"}";

	const char* color_fshader =
		"#version 400\n"
		"in VertexData{"
		"  vec2 UV;"
		"  vec3 color;"
		"} inp;"
		"out vec4 frag_color;"
		"void main() {"
		"  frag_color = vec4(inp.color, 1.0);"
		"}";

	const char* texture_fshader =
		"#version 400\n"
		"in VertexData{"
		"  vec2 UV;"
		"  vec3 color;"
		"} inp;"
		"out vec4 frag_color;"
		"uniform sampler2D tex;"
		"void main() {"
		"  frag_color = texture( tex, inp.UV );"
		"}";

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
