#pragma once

#include <map>
#include "ComponentManager.h"
#include "Mesh.h"

using namespace std;

//using structs with arrays to store data for all entities together, next to eachother in memory so that all the needed data is quickly accessible for all systems by just using the index the data is stored in
//(which is the entity's ID)
//all of the arrays also have to be initialised with a defined size. for the demo I'll use 20 as I plan on having around 15 entities, but for expanded use you could just set it to a big number
//as it should still be faster for the CPU to have dedicated memory locations for all this data rather than adaptive memory locations such as with a vector. So it would essentially trade
//memory usage for speed which would be a worthwhile trade since the memory usage should be minimal compared to the speed.

//all code is based on the code found in "Entity Component System and DOD relationship - Part II.pdf" on canvas

//positional data for all entities, stores values in XYZ
struct PositionComponent {
public:
	float x[20];
	float y[20];
	float z[20];
};

//Mesh data for all entities
struct RendringComponent {
public:
	Mesh meshes[20];
};

//Stores the velocity of each entity, stores the XYZ velocities in their own arrays.
struct VelocityComponent {
public:
	float x[20];
	float y[20];
	float z[20];
};

struct SizeComponent {
public:
	float x[20];
	float y[20];
	float z[20];
};

struct GravityComponent {
public:
	bool hasGravity;
};

struct CollideableComponent {
public:
	bool canCollide;
};

struct PhysicsComponent {
public:
	float mass[20];
};

//base code taken from "Entity Component System and DOD relationship - Part II.pdf" on canvas.
template<typename T> class ComponentManager
{
private:
	map<int, T> components;
public:
	void addComponent(int ID, T component) {
		components[ID] = component;
	}
	T getComponent(int ID) {
		return components[ID];
	}
	bool hasComponent(int ID) {
		return components.find(ID) != components.end();
	}
	void removeComponent(int ID) {
		components.erase(ID);
	}
	map<int, T> getAll(int ID) {
		return components;
	}
};

