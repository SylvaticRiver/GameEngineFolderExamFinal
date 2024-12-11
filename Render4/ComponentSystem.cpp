#include "ComponentSystem.h"
#include <iostream>

MovementSystem::MovementSystem(ComponentManager<PositionComponent>* position, ComponentManager<VelocityComponent>* velocity, ComponentManager<GravityComponent> gravity)
{
	this->component = position;
	this->velocity = velocity;
	this->gravity = gravity;
}

RendringSystem::RendringSystem(ComponentManager<PositionComponent>* position, ComponentManager<RendringComponent> render, ComponentManager<SizeComponent> size)
{
	this->Positioncomponent = position;
	this->Rendringcomponent = render;
	this->Sizecomponent = size;
}


PhysicsSystem::PhysicsSystem(ComponentManager<PositionComponent>* position, ComponentManager<VelocityComponent>* velocity, ComponentManager<PhysicsComponent> physics, ComponentManager<CollideableComponent> collideable, ComponentManager<RendringComponent> mesh, ComponentManager<SizeComponent> size)
{
	this->Positionpomponent = position;
	this->Velcoitycomponent = velocity;
	this->Physicscomponent = physics;
	this->Collideablecomponent = collideable;
	this->Renderingcomponent = mesh;
	this->Sizecomponent = size;
}

//this one is very long since all the collision functionality, both detection and response, for both ball-ball and ball-wall are implemented here.
//I could make it separate components, but I ran out of time.
void PhysicsSystem::update(list<Entity> entity)
{
	for (Entity ent : entity) {
		if (Physicscomponent.hasComponent(ent.id) && Collideablecomponent.hasComponent(ent.id)) {
			if (Positionpomponent->hasComponent(ent.id) && Velcoitycomponent->hasComponent(ent.id) && Renderingcomponent.hasComponent(ent.id) && Sizecomponent.hasComponent(ent.id)) {
				PositionComponent entPos = Positionpomponent->getComponent(ent.id);
				VelocityComponent entVel = Velcoitycomponent->getComponent(ent.id);
				SizeComponent entSize = Sizecomponent.getComponent(ent.id);
				float entMass = Physicscomponent.getComponent(ent.id).mass[ent.id];
				Mesh entMesh = Renderingcomponent.getComponent(ent.id).meshes[ent.id];
				for (Entity other : entity) {
					if (other.id != ent.id) {
						if (Collideablecomponent.hasComponent(other.id) && Renderingcomponent.hasComponent(other.id) && Positionpomponent->hasComponent(other.id) && Sizecomponent.hasComponent(other.id)) {
							PositionComponent otherPos = Positionpomponent->getComponent(other.id);
							SizeComponent otherSize = Sizecomponent.getComponent(other.id);
							//if the other entity has a physics component, then it is a ball and we apply ball-ball physics
							if (Physicscomponent.hasComponent(other.id) && Velcoitycomponent->hasComponent(other.id)) {
								VelocityComponent otherVel = Velcoitycomponent->getComponent(other.id);
								float otherMass = Physicscomponent.getComponent(other.id).mass[other.id];

								glm::vec3 entCurrPos(entPos.x[ent.id], entPos.y[ent.id], entPos.z[ent.id]);
								glm::vec3 otherCurrPos(otherPos.x[other.id], otherPos.y[other.id], otherPos.z[other.id]);

								glm::vec3 entCurrVel(entVel.x[ent.id], entVel.y[ent.id], entVel.z[ent.id]);
								glm::vec3 otherCurrVel(otherVel.x[other.id], otherVel.y[other.id], otherVel.z[other.id]);

								static const float EPSILON{ 0.000001 };
								glm::vec3 A = entCurrPos - otherCurrPos;
								glm::vec3 B = entCurrVel - otherCurrVel;

								float AB = A.x * B.x + A.y * B.y + A.z * B.z;
								float AA = A.x * A.x + A.y * A.y + A.z * A.z;
								float BB = B.x * B.x + B.y * B.y + B.z * B.z;

								float d = 0.1f + 0.1f;
								float rot = (AB * AB) - (BB * (AA - d * d));
								float t = -1.0f;
								if (rot >= 0.0f) {
									t = -AB - std::sqrt(rot);
								}
								if (BB > EPSILON) {
									t = t / BB;
								}
								//run ball-ball collisionresponse if 0 < t < 1
								if (0 < t && t < 1) {
									glm::vec3 v0 = entCurrVel * (entMass - otherMass) + otherCurrVel * otherMass * 2.0f;
									v0 = v0 / (otherMass + entMass);
									glm::vec3 v1 = entCurrVel * (otherMass - entMass) + entCurrVel * entMass * 2.0f;
									v1 = v1 / (entMass + otherMass);
									cout << "hit" << endl;
									entCurrPos = entCurrPos - entCurrVel;
									entCurrVel = v0;
									otherCurrPos = otherCurrPos - otherCurrVel;
									otherCurrVel = v1;

									entVel.x[ent.id] = entCurrVel.x;
									entVel.y[ent.id] = entCurrVel.y;
									entVel.z[ent.id] = entCurrVel.z;

									entPos.x[ent.id] = entCurrPos.x;
									entPos.y[ent.id] = entCurrPos.y;
									entPos.z[ent.id] = entCurrPos.z;

									otherVel.x[ent.id] = otherCurrVel.x;
									otherVel.y[ent.id] = otherCurrVel.y;
									otherVel.z[ent.id] = otherCurrVel.z;

									otherPos.x[ent.id] = otherCurrPos.x;
									otherPos.y[ent.id] = otherCurrPos.y;
									otherPos.z[ent.id] = otherCurrPos.z;

									Positionpomponent->addComponent(ent.id, entPos);
									Velcoitycomponent->addComponent(ent.id, entVel);
									Positionpomponent->addComponent(ent.id, otherPos);
									Velcoitycomponent->addComponent(ent.id, otherVel);
								}
							}
							else {
								Mesh otherMesh = Renderingcomponent.getComponent(other.id).meshes[other.id];
								glm::vec3 entCurrPos(entPos.x[ent.id], entPos.y[ent.id], entPos.z[ent.id]);
								glm::vec3 otherCurrPos(otherPos.x[other.id], otherPos.y[other.id], otherPos.z[other.id]);

								glm::vec3 entCurrSize(entSize.x[ent.id], entSize.y[ent.id], entSize.z[ent.id]);
								glm::vec3 otherCurrSize(otherSize.x[other.id], otherSize.y[other.id], otherSize.z[other.id]);

								glm::vec3 entCurrVel(entVel.x[ent.id], entVel.y[ent.id], entVel.z[ent.id]);
								for (Triangle t : otherMesh.getTriangles()) {
									//calculates the position of the triangle because the walls have been both scaled and moved, which has to be accounted for in the calculations.
									Vertex v1 = t.verticies[0];
									Vertex v2 = t.verticies[1];
									Vertex v3 = t.verticies[2];
									glm::vec3 TriPos = glm::vec3((v1.pos + v2.pos).x / 2, (v1.pos + v2.pos).y / 2, (v1.pos + v2.pos).z / 2);
									TriPos = glm::vec3((TriPos + v3.pos).x / 2, (TriPos + v3.pos).y / 2, (TriPos + v3.pos).z / 2);

									//calculating the normal of the walls and floor
									glm::vec3 normal = glm::cross(v2.pos - v1.pos, v3.pos - v1.pos);

									//ball collides if |y| <= r. 
									glm::vec3 y = glm::vec3(entCurrPos.x - ((TriPos.x * otherCurrSize.x) + otherCurrPos.x), entCurrPos.y - ((TriPos.y * otherCurrSize.y) + otherCurrPos.y), entCurrPos.z - ((TriPos.z * otherCurrSize.z) + otherCurrPos.z));
									float r = (glm::normalize(entMesh.vertices[0].pos + entMesh.vertices[1].pos) * entCurrSize).x + 0.1;
									float f = glm::l1Norm((y * normal));
									if (f <= r) {
										//collisionresponse ball-wall
										//cout << "hit" << endl;
										//chapter 9.7.3, figure 9.9: y = ((P - P0) * n) * n
										glm::vec3 y = (glm::vec3(entCurrPos.x - ((TriPos.x * otherCurrSize.x) + otherCurrPos.x), entCurrPos.y - ((TriPos.y * otherCurrSize.y) + otherCurrPos.y), entCurrPos.z - ((TriPos.z * otherCurrSize.z) + otherCurrPos.z)) * normal) * normal;
										glm::vec3 a = (entCurrVel * normal) * normal;
										glm::vec3 x = entCurrVel - (glm::vec3(0.125 * a.x, 0.125 * a.y, 0.125 * a.z));

										//av en eller annen grunn teleporterer dette ballene out of bounds, saa jeg bruker heller bare den forrige posisjonsdataen til ballen
										entCurrPos = entCurrPos - entCurrVel;
										//this->currPos = this->prevPos;
										//entCurrPos.y += 0.1;
										entCurrVel = x * glm::vec3(0.95);

										entVel.x[ent.id] = entCurrVel.x;
										entVel.y[ent.id] = entCurrVel.y;
										entVel.z[ent.id] = entCurrVel.z;

										entPos.x[ent.id] = entCurrPos.x;
										entPos.y[ent.id] = entCurrPos.y;
										entPos.z[ent.id] = entCurrPos.z;

										Positionpomponent->addComponent(ent.id, entPos);
										Velcoitycomponent->addComponent(ent.id, entVel);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//The rendering system takes does not override the update fuction from the ComponentManager class since I needed the Shader and camera as arguments, and for some reason, the shader refused to
//work as a variable in the RenderingComponent class. And so, since I already had to make it not override its super function, I decided to forego trying to add the camera and bool as variables too

//The component is supposed to store an array of meshes, but for some reason, the meshes would only store to slot 0 no matter what. Luckily, all the entities were planned to have the same mesh regardless
//So it will still use entity ID to pull out a mesh from the Rendring component, but it will take out from the same mesh in slot 0.

//it updates the mesh's position by using the PositionComponent's stored data for each entityID in each coordinate. The Rendering system also uses the HealthComponent to determine if the entity is dead,
//if the entity is dead, it should no longer render

void RendringSystem::update(list<Entity> entity, Shader& shaderProgram, Camera& camera, bool dotRender)
{
	for (Entity ent : entity) {
		if (Rendringcomponent.hasComponent(ent.id) && Positioncomponent->hasComponent(ent.id)) {
			PositionComponent position = Positioncomponent->getComponent(ent.id);
			glm::vec3 size(0.3);
			if (Sizecomponent.hasComponent(ent.id)) {
				SizeComponent entSize = Sizecomponent.getComponent(ent.id);
				size = glm::vec3(entSize.x[ent.id], entSize.y[ent.id], entSize.z[ent.id]);
			}
			Rendringcomponent.getComponent(ent.id).meshes[ent.id].VAO.Bind();
			glm::mat4 objectModel = glm::mat4(1.0f);
			objectModel = glm::scale(glm::translate(objectModel, glm::vec3(position.x[ent.id], position.y[ent.id], position.z[ent.id])), size);
			objectModel = glm::rotate(objectModel, glm::radians(0.0f), glm::vec3(0, 0, 1));
			shaderProgram.Activate();
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));

			Rendringcomponent.getComponent(ent.id).meshes[ent.id].Draw(shaderProgram, camera, dotRender);
		}
	}
}

//System for updating their movement. It simply adds their velocity to their current position, and then decreases velocity significantly. This makes it so that when the entity is ordered to move it will
//move with the amount of the given velocity, but once it's ordered to stop it will drift for a few frames before stopping to a halt.

void MovementSystem::update(list<Entity> entity)
{
	for (Entity ent : entity) {
		if (component->hasComponent(ent.id) && velocity->hasComponent(ent.id)) {
			PositionComponent position = component->getComponent(ent.id);
			VelocityComponent vel = velocity->getComponent(ent.id);

			if (gravity.hasComponent(ent.id)) {
				if (vel.y[ent.id] > -0.04) {
					vel.y[ent.id] -= 0.0004f;
				}
			}

			position.x[ent.id] += vel.x[ent.id];
			position.y[ent.id] += vel.y[ent.id];
			position.z[ent.id] += vel.z[ent.id];

			component->addComponent(ent.id, position);
			velocity->addComponent(ent.id, vel);
		}
	}
}

void ComponentSystem::update(list<Entity> entity)
{

}

