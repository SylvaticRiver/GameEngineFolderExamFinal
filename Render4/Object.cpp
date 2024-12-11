#include "Object.h"

Object::Object()
{

}

Object::Object(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, Rotator rotation, Shader& shaderProgram, bool hasGravity)
{
	this->mesh = mesh;
	this->currPos = startPos;
	this->size = size;
	this->rotation = rotation;
	this->velocity = glm::vec3(0, 0, 0);
	this->hasGravity = hasGravity;
	this->prevPos = this->currPos;

	this->mesh.VAO.Bind();
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::scale(glm::translate(objectModel, this->currPos), this->size);
	objectModel = glm::rotate(objectModel, glm::radians(this->rotation.amount), this->rotation.Axis);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));

	this->model = objectModel;
}

void Object::Render(Shader& shader, Camera& camera, bool dotRender)
{
	updateMovement();
	update(shader);
	this->mesh.Draw(shader, camera, dotRender);
}

void Object::fullRender(Shader& shader, Camera& camera, GLFWwindow* window, bool dotRender)
{
	update(shader);
	Render(shader, camera, dotRender);
	processInputs(window);
}

void Object::setPos(glm::vec3 newPos)
{
	this->currPos = newPos;
}

void Object::move(glm::vec3 amount)
{
	this->currPos += amount;
}

void Object::rescale(glm::vec3 newSize)
{
	this->size = newSize;
}

void Object::rotate(float degree, glm::vec3 direction)
{
	this->rotation = Rotator{direction, degree};
}

void Object::processInputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		this->move(glm::vec3(0.0f, 0.0f, 0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		this->move(glm::vec3(0.0f, 0.0f, -0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		this->move(glm::vec3(0.01f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		this->move(glm::vec3(-0.01f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		this->move(glm::vec3(0.0f, -0.01f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		this->move(glm::vec3(0.0f, 0.01f, 0.0f));
	}
}

void Object::update(Shader& shaderProgram)
{
	this->mesh.VAO.Bind();
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::scale(glm::translate(objectModel, this->currPos), this->size);
	objectModel = glm::rotate(objectModel, glm::radians(this->rotation.amount), this->rotation.Axis);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
}

std::vector<Triangle> Object::getTriangles() {

	std::vector<Triangle> tris;
	std::vector<Vertex> verts = this->mesh.vertices;
	std::vector<GLuint> ind = this->mesh.indices;

	for (int i = 0; i < ind.size() / 3; i++) {
		std::vector<Vertex> triverts;
		for (int j = 0; j < 3; j++) {
			triverts.push_back(verts[ind[(3 * i) + j]]);
		}
		Triangle t{ triverts };
		tris.push_back(t);
	}

	return tris;
}

float Object::getBarHeight(Triangle triangle, glm::vec3 bars) {
	return (bars[0] * triangle.verticies[0].pos[2]) + (bars[1] * triangle.verticies[1].pos[2]) + (bars[2] * triangle.verticies[2].pos[2]);
}

//taken from math 3 notes on canvas
glm::vec3 Object::barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 p12 = p2 - p1;
	glm::vec3 p13 = p3 - p1;
	glm::vec3 n = glm::cross(p12, p13);

	float areal_123 = n.length();
	glm::vec3 baryc;

	glm::vec3 p = p2 - this->currPos;
	glm::vec3 q = p3 - this->currPos;
	n = glm::cross(p, q);
	baryc.x = n.z / areal_123;

	p = p3 - this->currPos;
	q = p1 - this->currPos;
	n = glm::cross(p, q);
	baryc.y = n.z / areal_123;

	p = p1 - this->currPos;
	q = p2 - this->currPos;
	n = glm::cross(p, q);
	baryc.z = n.z / areal_123;

	return baryc;
}

void Object::updateMovement()
{
	this->prevPos = this->currPos;
	move(this->velocity);
}

LightObject::LightObject(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, Rotator rotation, glm::vec4 lightColor, Shader& lightShader, Shader& shaderProgram) {
	LightObject::mesh = mesh;
	LightObject::currPos = startPos;
	LightObject::size = size;
	LightObject::rotation = rotation;
	LightObject::lightcolor = lightColor;

	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, LightObject::currPos);
	lightModel = glm::scale(glm::translate(lightModel, LightObject::currPos), LightObject::size);
	lightModel = glm::rotate(lightModel, glm::radians(LightObject::rotation.amount), LightObject::rotation.Axis);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), LightObject::lightcolor.x, LightObject::lightcolor.y, LightObject::lightcolor.z, LightObject::lightcolor.w);
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), LightObject::lightcolor.x, LightObject::lightcolor.y, LightObject::lightcolor.z, LightObject::lightcolor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), LightObject::currPos.x, LightObject::currPos.y, LightObject::currPos.z);

	LightObject::model = lightModel;
}
