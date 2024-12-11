#pragma once

#include "glm/vec3.hpp"

#include "Mesh.h"
#include "test.h"

//example code from canvas "ParticleSystemDoD.cpp"
struct ParticleEmitter
{
	static const int maxParticles = 500;

	float posX[maxParticles];
	float posY[maxParticles];
	float posZ[maxParticles];

	float velX[maxParticles];
	float velY[maxParticles];
	float velZ[maxParticles];

	float lifetime[maxParticles];

	int count = 0;

	Mesh meshes[maxParticles];

	void spawnParticle(Mesh& mesh, glm::vec3 spawnPos, glm::vec3 spawnVel);
	void updateParticles();
	void renderParticles(Shader& shader, Camera& camera);
};
