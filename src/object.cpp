#include <object.h>
#include <shader.h>
#include <utils.h>
#include <fstream>
#include <vector>

Object::Object() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO);
	
}
Object::~Object() {}

/// <summary>
/// 将点坐标载入VBO，必须在传入顶点坐标到vector后调用
/// </summary>
void Object::init() {
	assert(vertices.size() > 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) * 2, &(this->vertices)[0], GL_STATIC_DRAW);
}


/// <summary>
/// draw object with VAO and VBO. 
/// layout: 0 为坐标 2 为法线
/// </summary>
void Object::drawArrays() const {
	
	//载入顶点坐标
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type*
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)0            // array buffer offset
	);
	//载入顶点法线
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute
		3,                  // size
		GL_FLOAT,           // type*
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)(3 * sizeof(float))            // array buffer offset
	);
	
	glDrawArrays(draw_mode.primitive_mode, 0, vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

/* Implement this one if you do use a shader */
void Object::drawArrays(const Shader& shader) const {
  // !DELETE THIS WHEN YOU FINISH THIS FUNCTION
  UNIMPLEMENTED;
}

/**
 * TODO: draw object with VAO, VBO, and VEO
 * You can choose to implement either one or both of the following functions.
 */

/* Implement this one if you do not use a shader */
void Object::drawElements() const {
  // !DELETE THIS WHEN YOU FINISH THIS FUNCTION
  UNIMPLEMENTED;
}

/* Implement this one if you do use a shader */
void Object::drawElements(const Shader& shader) const {
  // !DELETE THIS WHEN YOU FINISH THIS FUNCTION
  UNIMPLEMENTED;
}