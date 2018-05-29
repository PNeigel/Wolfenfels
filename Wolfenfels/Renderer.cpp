#include "Renderer.h"
#include "globalenums.h"

#include <png.hpp>


Renderer::Renderer(Stage & stage, Enemy & enemy)
{	
}

Renderer::Renderer(vector<GLuint>* shader) :
	shader(shader)
{
}

Renderer::~Renderer()
{
}

void Renderer::RenderStageWalls(Stage & stage, GLuint shader)
{
	glUseProgram(shader);

	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &stage.player.mvp[0][0]);

	stage.m_wallModel->m_texture->bind();
	stage.m_wallModel->bindVAO();
	glDrawArrays(GL_QUADS, 0, ResourceManager::m_wallMesh.size() / 3);
}

void Renderer::RenderBG(Stage & stage, GLuint shader)
{
	glUseProgram(shader);
	stage.m_bgModel->bindVAO();
	glDrawArrays(GL_QUADS, 0, 8);
}

void Renderer::RenderAll(Stage & stage, GLuint* shader)
{
	RenderBG(stage, shader[Shader::COLOR_SCREEN]);
	RenderStageWalls(stage, shader[Shader::TEXTURE_PROJ]);
	for (Door & door : stage.m_doors)
		renderDoor(stage.player, door, shader[Shader::TEXTURE_PROJ]);
	if (stage.enemies.size() > 0) {
		for (auto enemy_pointer = stage.enemies.rbegin(); enemy_pointer != stage.enemies.rend(); enemy_pointer++) {
			RenderEnemy(stage.player, *enemy_pointer, shader[Shader::TEXTURE_PROJ]);
		}
	}
	RenderPlayer(stage.player, shader[Shader::TEXTURE_SCREEN]);
}

void Renderer::RenderPlayer(Player & player, GLuint shader)
{
	glUseProgram(shader);

	player.model->m_texture->bind();
	player.model->bindVAO();
	glDrawArrays(GL_QUADS, 0, 4);
}

void Renderer::RenderEnemy(Player & player, Enemy & enemy, GLuint shader)
{
	glUseProgram(shader);

	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	glm::mat4 mvp = player.mvp * enemy.model_matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	enemy.model->m_texture->bind();
	enemy.model->updateVBO(1, enemy.m_UV.size(), enemy.m_UV.data());
	enemy.model->bindVAO();
	glDrawArrays(GL_QUADS, 0, 4);
}

void Renderer::renderDoor(Player & player, Door & door, GLuint shader)
{
	glUseProgram(shader);

	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	glm::mat4 mvp = player.mvp * door.m_modelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	door.m_model->m_texture->bind();
	door.m_model->bindVAO();
	glDrawArrays(GL_QUADS, 0, ResourceManager::m_doorMesh.size() / 3);
}

void Renderer::RenderLine(Player & player, GLuint shader, glm::vec3 start, glm::vec3 end)
{
	float linedata[] = { start.x, start.y, start.z,
						end.x, end.y, end.z };

	float colordata[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLuint vbo_pos = 0;
	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), linedata, GL_STATIC_DRAW);

	GLuint vbo_color = 0;
	glGenBuffers(1, &vbo_color); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), colordata, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(shader);

	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &player.mvp[0][0]);

	glLineWidth(3.0);
	glDrawArrays(GL_LINES, 0, 2);

	GLuint delete_array[] = { vbo_pos, vbo_color};

	glDeleteBuffers(2, delete_array);
	glDeleteVertexArrays(1, &vao);
}
