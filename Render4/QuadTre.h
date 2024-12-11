#pragma once

#include <vector>
#include "Object.h"
#include "PhysicsObject.h"

//made using pseudocode from VSIM notes on canvas chapter 8.4, Listing 8.5 - 8.9
class QuadTre
{
public:
	struct Point2F {
		float x;
		float z;
	};
public:
	Point2F A;
	Point2F B;
	Point2F C;
	Point2F D;

	QuadTre* sv;
	QuadTre* so;
	QuadTre* no;
	QuadTre* nv;
	QuadTre* parent;

	vector<PhysicsObject*> objects;
	bool isLeaf;
public:

	std::vector<QuadTre*> quads;

	QuadTre();
	QuadTre(Point2F &a, Point2F &b, Point2F &c, Point2F &d, bool isLeaf);
	Point2F findMiddle(Point2F &a, Point2F &b);
	void subDivide(int n);
	void print();
	QuadTre* insert(PhysicsObject* object);
	QuadTre* find(Point2F& point);
	vector<PhysicsObject*> getObjects();
	void clearObjects();
	bool hasObjects();
};

