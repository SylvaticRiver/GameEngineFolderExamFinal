#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>
#include <iostream>
#include <vector>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

struct Triangle {
	std::vector<Vertex> verticies;
};

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	VAO VAO;
	glm::vec3 pos;

	Mesh();
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	void Draw(Shader& shader, Camera& camera, bool dotRender);
	std::vector<Triangle> getTriangles();
};
#endif
