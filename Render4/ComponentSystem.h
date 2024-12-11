#pragma once

#include "ComponentManager.h"
#include <list>

//all code is based on "Entity Component System and DOD relationship - Part II.pdf" in canvas

//Entity as a struct with an ID to navigate in contigous memory
struct Entity {
	int id;
};

//I was unable to make this purely an abstract class/interface since I'm used to the strict "interface" keyword in Java. 
//But I believe this still simulates an interface well enough by not having
//any code in the update function, which essentially is the same as not having any implementation
class ComponentSystem
{
public:
	virtual void update(list<Entity> entity);
};

//System for calculating movement. It uses the entities' current position and their velocity to assign the new position data to the PositionComponent apropriately
class MovementSystem : ComponentSystem
{
private:
	ComponentManager<PositionComponent>* component;
	ComponentManager<VelocityComponent>* velocity;
	ComponentManager<GravityComponent> gravity;

public:
	MovementSystem(ComponentManager<PositionComponent>* position, ComponentManager<VelocityComponent>* velocity, ComponentManager<GravityComponent> gravity);

	void update(list<Entity> entity) override;
};

//System for rendering. It uses Position data to determine where to draw the meshes, as well as the entities' health data to determine whether they should be rendered or not (simulating that they're dead)
//it could not override the update function since it needed extra arguments. And the Position and Health components needed to be pointers since it needed the updated references from memory
//rather than a copy of the fresh components created during initialisation. The Rendering component doesn't need to be a pointer since the data stored in it is never updated, only referenced
class RendringSystem : ComponentSystem
{
private:
	ComponentManager<PositionComponent>* Positioncomponent;
	ComponentManager<RendringComponent> Rendringcomponent;
	ComponentManager<SizeComponent> Sizecomponent;

public:
	RendringSystem(ComponentManager<PositionComponent>* position, ComponentManager<RendringComponent> render, ComponentManager<SizeComponent> size);

	void update(list<Entity> entity, Shader& shaderProgram, Camera& camera, bool dotRender);
};

class PhysicsSystem : ComponentSystem
{
private:
	ComponentManager<PositionComponent>* Positionpomponent;
	ComponentManager<VelocityComponent>* Velcoitycomponent;
	ComponentManager<PhysicsComponent> Physicscomponent;
	ComponentManager<CollideableComponent> Collideablecomponent;
	ComponentManager<RendringComponent> Renderingcomponent;
	ComponentManager<SizeComponent> Sizecomponent;

public:
	PhysicsSystem(ComponentManager<PositionComponent>* position, ComponentManager<VelocityComponent>* velocity,
		ComponentManager<PhysicsComponent> physics, ComponentManager<CollideableComponent> collideable,
		ComponentManager<RendringComponent> mesh, ComponentManager<SizeComponent> size);

	void update(list<Entity> entity) override;
};
