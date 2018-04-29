#include <iostream>
#include <sstream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Stage.h"

using namespace std;

glm::vec3 camera_pos{ 0.0f, -3.0f, 0.5f };
float campos_delta = 0.1f;

float yaw = 0.0f;
float yaw_delta = 5.0f;

glm::mat4 View{ 1.0f };
glm::vec3 view_dir{ 1.0f };

void ComputeView() {
	View = glm::mat4(1.0f);
	View = glm::rotate(glm::radians(90.0f), glm::vec3{ 1,0,0 }) * View;
	View = glm::translate(camera_pos) * View;
	View = View * glm::rotate(glm::radians(yaw), glm::vec3{ 0,1,0 });
	view_dir = View * glm::vec4{ 0,0,-1,0 };
	View = glm::inverse(View);
}

void MoveCamera(int keytype) {
	if (keytype == GLFW_KEY_UP) camera_pos += view_dir * campos_delta;
	else if (keytype == GLFW_KEY_DOWN) camera_pos -= view_dir * campos_delta;
}

void ChangeYaw(int keytype) {
	if (keytype == GLFW_KEY_LEFT) yaw += yaw_delta;
	else if (keytype == GLFW_KEY_RIGHT) yaw -= yaw_delta;
	if (yaw > 360) yaw -= 360;
	if (yaw < 0) yaw += 360;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float newang = 0;
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		cout << "Received input" << endl;
		switch (key) {
		case GLFW_KEY_UP : case GLFW_KEY_DOWN :
			MoveCamera(key);
			break;
		case GLFW_KEY_LEFT: case GLFW_KEY_RIGHT:
			ChangeYaw(key);
		default:
			break;
		}
	}
}

void DrawWall()
{
	float points[] = {
		-0.5, 0.0, -0.5,
		0.5, 0.0, -0.5,
		0.5, 0.0, 0.5,
		-0.5, 0.0, 0.5
	};

	float colors[] = {
		1.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 0.0f,  1.0f,
		0.9f, 0.8f,  0.7f
	};

	glEnable(GL_DEPTH_TEST);


	// Vertex buffer objects and Vertex array objects
	GLuint vbo_pos = 0;
	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint vbo_col = 0;
	glGenBuffers(1, &vbo_col); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 4);
}

int main()
{
	GLFWwindow* window;

	int width = 1366;//640*2;
	int height = 768;//480 * 2;

	// Initialize GLFW
	if (!glfwInit()) {
		cout << "Error: Couldn't initialize GLFW." << endl;
		return -1;
	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	// Each thread can have only one current context at a time
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported: " << version << endl;
	
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glm::mat4 Projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);
	/*
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0, -5.0, 0.5), // Camera is at (x,y,z), in World Space
		glm::vec3(0.0, 3.0, 0.0), // and looks at the origin
		glm::vec3(0.0, 0.0, 1.0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	*/

	ComputeView();

	glm::mat4 mvp = Projection * View;


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

	GLuint shader_programme = glCreateProgram();

	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	double player_x = 0.5;
	double player_y = 5;
	double player_z = 0;

	glfwSetKeyCallback(window, key_callback);

	Stage mystage{ 1 };

	// Loop until the user closes the window
	double elapsed = 0;
	uint32_t framecount = 0;
	while (!glfwWindowShouldClose(window)) {
		double start_time = glfwGetTime();
		// wipe the drawing surface clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		GLuint MatrixID = glGetUniformLocation(shader_programme, "MVP");

		ComputeView();
		glm::mat4 mvp = Projection * View;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		//glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glEnable(GL_POINT_SMOOTH);
		//glPointSize(15.0f);
		//glDrawArrays(GL_LINE_LOOP, 0, 4);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
		//DrawWall();
		mystage.RenderStage();

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
		// update other events like input handling 
		glfwPollEvents();

		// Calculate and display FPS
		framecount++;
		elapsed += glfwGetTime() - start_time;
		if (elapsed > 0.5) {
			stringstream wintitle;
			wintitle << "FPS: " << framecount / elapsed;
			glfwSetWindowTitle(window, wintitle.str().c_str());
			elapsed = 0;
			framecount = 0;
		}
	}

	glfwTerminate();
}