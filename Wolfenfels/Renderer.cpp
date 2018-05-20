#include "Renderer.h"
#include "globalenums.h"

#include <png.hpp>


Renderer::Renderer(Stage & stage, Player & player, Enemy & enemy)
{
	BasicTexture* wall_texture = new BasicTexture("Assets/wall.png");
	m_textures.push_back(wall_texture);

	bg_vao = VAllocBG(stage);
	stage_walls_vao = VAllocStageWalls(stage);

	TextureAtlas* player_texture = new TextureAtlas("Assets/pistol.png", 1, 5);
	m_textures.push_back(player_texture);
	player_vao = VAllocPlayersprite(player);

	BasicTexture* enemy_texture = new BasicTexture("Assets/Cyclops.png");
	m_textures.push_back(enemy_texture);
	enemy_vao = VAllocEnemy(enemy);

}

Renderer::Renderer(vector<GLuint>* shader) :
	shader(shader)
{
}


Renderer::~Renderer()
{
}



GLuint Renderer::VAllocStageWalls(Stage & stage)
{
	GLuint vbo_pos = 0;
	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, stage.n_vertcoords * sizeof(float), stage.vertcoords, GL_STATIC_DRAW);

	GLuint vbo_uv = 0;
	glGenBuffers(1, &vbo_uv); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, stage.n_wallverts * 2 * sizeof(float), stage.wall_UV_coords, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	return vao;
}

GLuint Renderer::VAllocBG(Stage & stage)
{
	GLuint vbo_pos = 0;
	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), stage.bgverts, GL_STATIC_DRAW);

	GLuint vbo_col = 0;
	glGenBuffers(1, &vbo_col); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), stage.bgcolors, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	return vao;
}

void Renderer::RenderStageWalls(Stage & stage, Player & player, GLuint shader, GLuint vao)
{
	glUseProgram(shader);

	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &player.mvp[0][0]);

	m_textures[0]->bind();
	//glBindTexture(GL_TEXTURE_2D, wall_textureID);
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, stage.n_wallverts);
}

void Renderer::RenderBG(Stage & stage, GLuint shader, GLuint vao)
{
	// Dont really need stage at the moment
	glUseProgram(shader);
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 8);
}

void Renderer::RenderAll(Stage & stage, Player & player, GLuint* shader)
{
	RenderBG(stage, shader[Shader::COLOR_SCREEN], bg_vao);
	RenderStageWalls(stage, player, shader[Shader::TEXTURE_PROJ], stage_walls_vao);
	if (player.weapon_anim.playing) {
		RenderLine(player, shader[Shader::COLOR_PROJ], glm::vec3{ player.pos.x, player.pos.y, 0.3 } + player.view_dir * 0.1, player.pos + player.view_dir * 10.0f);
		/*
		glm::vec2 perp{ enemy.dir.y * -1.0f, enemy.dir.x };
		glm::vec2 right = glm::vec2(enemy.pos) + perp * enemy.coll_width / 2.0;
		glm::vec2 left = glm::vec2(enemy.pos) - perp * enemy.coll_width / 2.0;
		RenderLine(player, shader[Shader::COLOR_PROJ], glm::vec3{ player.pos.x, player.pos.y, 0.3 }, glm::vec3(right, 0.5));
		RenderLine(player, shader[Shader::COLOR_PROJ], glm::vec3{ player.pos.x, player.pos.y, 0.3 }, glm::vec3(left, 0.5));
		RenderLine(player, shader[Shader::COLOR_PROJ], glm::vec3{ player.pos.x, player.pos.y, 0.3 }, enemy.pos);
		*/
	}
	if (stage.enemies.size() > 0) {
		for (Enemy & enemy : stage.enemies) {
			RenderEnemy(player, enemy, shader[Shader::TEXTURE_PROJ], enemy_vao);
		}
	}
	RenderPlayer(player, shader[Shader::TEXTURE_SCREEN], player_vao);
}

GLuint Renderer::VAllocPlayersprite(Player & player)
{
	GLuint vbo_pos = 0;
	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), player.spritecoords, GL_STATIC_DRAW);

	vbo_player_uv = 0;
	glGenBuffers(1, &vbo_player_uv); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_player_uv); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), player.sprite_UV_coords, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_player_uv);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	return vao;
}

void Renderer::UpdatePlayerUV(float uv_xy[8])
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_player_uv);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), uv_xy, GL_STATIC_DRAW);
}

GLuint Renderer::VAllocEnemy(Enemy & enemy)
{
	GLuint vbo_pos = 0;
	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), enemy.mesh, GL_STATIC_DRAW);

	GLuint vbo_uv = 0;
	glGenBuffers(1, &vbo_uv); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), enemy.UV, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	return vao;
}

void Renderer::RenderPlayer(Player & player, GLuint shader, GLuint vao)
{
	UpdatePlayerUV(player.sprite_UV_coords);
	glUseProgram(shader);

	m_textures[1]->bind();
	//glBindTexture(GL_TEXTURE_2D, player_textureID);
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 4);
}

void Renderer::RenderEnemy(Player & player, Enemy & enemy, GLuint shader, GLuint vao)
{
	glUseProgram(shader);

	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	glm::mat4 mvp = player.mvp * enemy.model_matrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	m_textures[2]->bind();
	//glBindTexture(GL_TEXTURE_2D, enemy_texID);
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 4);
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
