#include "Renderer2D.h"


Renderer2D::Renderer2D(GLuint* shader) :
	m_screenShader(shader)
{
}


Renderer2D::~Renderer2D()
{
}

void Renderer2D::RenderStageWalls(Stage & stage, Player& player)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	vector<GLfloat> stageVertex;
	vector<GLfloat> colors;
	for (Wall& wall : stage.walls) {
		stageVertex.push_back(wall.sides[0].x);
		stageVertex.push_back(wall.sides[0].y);
		stageVertex.push_back(0);
		stageVertex.push_back(wall.sides[1].x);
		stageVertex.push_back(wall.sides[1].y);
		stageVertex.push_back(0);
		for (int i = 0; i < 6; i++) {
			colors.push_back(1.0);
		}
	}

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, stageVertex.size() * sizeof(GLfloat), stageVertex.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint cvbo = 0;
	glGenBuffers(1, &cvbo);
	glBindBuffer(GL_ARRAY_BUFFER, cvbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

	glUseProgram(*m_screenShader);

	GLuint scaleID = glGetUniformLocation(*m_screenShader, "scale");
	glUniform1f(scaleID, m_scale);
	GLuint aspectID = glGetUniformLocation(*m_screenShader, "aspectRatio");
	glUniform1f(aspectID, player.WIDTH / (float)player.HEIGHT);
	GLuint camPosID = glGetUniformLocation(*m_screenShader, "cam_pos");
	glUniform3fv(camPosID, 1, &player.pos[0]);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_LINES, 0, 2 * stage.walls.size());

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cvbo);
	glDeleteVertexArrays(1, &VAO);
}

void Renderer2D::RenderAll(Stage & stage, Player & player, CollisionHandler& coll)
{
	RenderStageWalls(stage, player);
	RenderPlayer(player);
	if (player.weapon_anim.playing)
		RenderRaycast(stage, player, coll);
	for (Enemy& enemy : stage.enemies) {
		RenderEnemy(player, enemy);
	}
}

void Renderer2D::RenderPlayer(Player & player)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLfloat x1 = player.pos.x - player.coll_width / 2.0;
	GLfloat x2 = player.pos.x + player.coll_width / 2.0;

	GLfloat y1 = player.pos.y - player.coll_height / 2.0;
	GLfloat y2 = player.pos.y + player.coll_height / 2.0;

	vector<GLfloat> playerVertex = { x1, y1, 0.0f,
									x2, y1, 0.0f,
									x2, y2, 0.0f,
									x1, y2, 0.0f,
									player.pos.x, player.pos.y, 0.0f,
									player.pos.x + 0.2f * player.view_dir.x, player.pos.y + 0.2f * player.view_dir.y, 0.0f };
	vector<GLfloat> colors;
	for (int i = 0; i < 6*3; i++) {
		colors.push_back(1.0f);
	}

	vector<GLuint> index = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, playerVertex.size() * sizeof(GLfloat), playerVertex.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint cvbo = 0;
	glGenBuffers(1, &cvbo);
	glBindBuffer(GL_ARRAY_BUFFER, cvbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

	GLuint ebo = 0;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLfloat), index.data(), GL_STATIC_DRAW);

	glUseProgram(*m_screenShader);

	GLuint scaleID = glGetUniformLocation(*m_screenShader, "scale");
	glUniform1f(scaleID, m_scale);
	GLuint aspectID = glGetUniformLocation(*m_screenShader, "aspectRatio");
	glUniform1f(aspectID, player.WIDTH / (float)player.HEIGHT);
	GLuint camPosID = glGetUniformLocation(*m_screenShader, "cam_pos");
	glUniform3fv(camPosID, 1, &player.pos[0]);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//glDrawArrays(GL_LINES, 0, 2 * 5);
	glDrawElements(GL_LINES, index.size(), GL_UNSIGNED_INT, NULL);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cvbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &VAO);
}

void Renderer2D::RenderEnemy(Player & player, Enemy & enemy)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glm::vec2 perp{-1.0f * enemy.dir.y, enemy.dir.x};
	glm::vec2 A = glm::vec2(enemy.pos) - perp * enemy.coll_width / 2.0f;
	glm::vec2 B = glm::vec2(enemy.pos) + perp * enemy.coll_width / 2.0f;
	vector<GLfloat> vertex = {A.x, A.y, 0.0f, B.x, B.y, 0.0f, enemy.pos.x, enemy.pos.y, 0.0f, enemy.pos.x + enemy.dir.x * 0.2f, enemy.pos.y + enemy.dir.y * 0.2f, 0.0f };
	vector<GLfloat> colors;
	for (int i = 0; i < 3 * vertex.size(); i++) {
		colors.push_back(1.0f);
	}

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint cvbo = 0;
	glGenBuffers(1, &cvbo);
	glBindBuffer(GL_ARRAY_BUFFER, cvbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

	glUseProgram(*m_screenShader);

	GLuint scaleID = glGetUniformLocation(*m_screenShader, "scale");
	glUniform1f(scaleID, m_scale);
	GLuint aspectID = glGetUniformLocation(*m_screenShader, "aspectRatio");
	glUniform1f(aspectID, player.WIDTH / (float)player.HEIGHT);
	GLuint camPosID = glGetUniformLocation(*m_screenShader, "cam_pos");
	glUniform3fv(camPosID, 1, &player.pos[0]);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_LINES, 0, 2 * 2);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cvbo);
	glDeleteVertexArrays(1, &VAO);
}

void Renderer2D::RenderRaycast(Stage & stage, Player & player, CollisionHandler& coll)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glm::vec2 A(player.m_lastRaycast[0]);
	glm::vec2 B(player.m_lastRaycast[1]);
	vector<GLfloat> vertex = { A.x, A.y, 0.0f, B.x, B.y, 0.0f };
	vector<GLfloat> colors;
	for (int i = 0; i < 3 * vertex.size(); i++) {
		colors.push_back(1.0f);
	}

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint cvbo = 0;
	glGenBuffers(1, &cvbo);
	glBindBuffer(GL_ARRAY_BUFFER, cvbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

	glUseProgram(*m_screenShader);

	GLuint scaleID = glGetUniformLocation(*m_screenShader, "scale");
	glUniform1f(scaleID, m_scale);
	GLuint aspectID = glGetUniformLocation(*m_screenShader, "aspectRatio");
	glUniform1f(aspectID, player.WIDTH / (float)player.HEIGHT);
	GLuint camPosID = glGetUniformLocation(*m_screenShader, "cam_pos");
	glUniform3fv(camPosID, 1, &player.pos[0]);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_LINES, 0, 2 * 2);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cvbo);
	glDeleteVertexArrays(1, &VAO);
}
