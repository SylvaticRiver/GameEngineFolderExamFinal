#include "ParticleEmitter.h"

void ParticleEmitter::spawnParticle(Mesh& mesh, glm::vec3 spawnPos, glm::vec3 spawnVel)
{
	if (count < maxParticles) {
		meshes[count] = mesh;
		posX[count] = spawnPos.x;
		posY[count] = spawnPos.y;
		posZ[count] = spawnPos.z;

		velX[count] = spawnVel.x;
		velY[count] = spawnVel.y;
		velZ[count] = spawnVel.z;

		lifetime[count] = 200;
		count++;
	}
}

void ParticleEmitter::updateParticles()
{
	for (int i = 0; i < count; i++) {
		posX[i] += velX[i];
		posY[i] += velY[i];
		posZ[i] += velZ[i];
		lifetime[i] -= 1;

		if (lifetime[i] <= 0) {
			count--;
			if (i != count) {
				meshes[i] = meshes[count];
				posX[i] = posX[count];
				posY[i] = posY[count];
				posZ[i] = posZ[count];

				velX[i] = velX[count];
				velY[i] = velY[count];
				velZ[i] = velZ[count];

				lifetime[i] = lifetime[count];
			}
		}
	}
}

void ParticleEmitter::renderParticles(Shader& shader, Camera& camera)
{
	for (int i = 0; i < count; i++) {
		meshes[i].VAO.Bind();

		glm::mat4 objectModel = glm::mat4(1.0f);
		objectModel = glm::scale(glm::translate(objectModel, glm::vec3(posX[i], posY[i], posZ[i])), glm::vec3(0.05, 0.05, 0.05));

		shader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));

		meshes[i].Draw(shader, camera, false);
	}
}
