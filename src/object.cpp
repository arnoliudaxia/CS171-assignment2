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
/// ������������VBO�������ڴ��붥�����굽vector�����
/// </summary>
void Object::init() {
	assert(vertices.size() > 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) * 2, &(this->vertices)[0], GL_STATIC_DRAW);
}


/// <summary>
/// draw object with VAO and VBO. 
/// layout: 0 Ϊ���� 2 Ϊ����
/// </summary>
void Object::drawArrays() const {
	
	//���붥������
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
	//���붥�㷨��
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