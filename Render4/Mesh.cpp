#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	VBO VBO(vertices);
	EBO EBO(indices);
	VAO.LinkVBO(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkVBO(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkVBO(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkVBO(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


void Mesh::Draw(Shader& shader, Camera& camera, bool dotRender)
{
	shader.Activate();
	VAO.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.Matrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	if (dotRender) {
		glPointSize(min(0.5f * (glm::distance(this->pos, camera.position) * 3), 5.0f));
		glDrawArrays(GL_POINTS, 0, this->vertices.size());
	}
}

std::vector<Triangle> Mesh::getTriangles() {

	std::vector<Triangle> tris;
	std::vector<Vertex> verts = this->vertices;
	std::vector<GLuint> ind = this->indices;

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
