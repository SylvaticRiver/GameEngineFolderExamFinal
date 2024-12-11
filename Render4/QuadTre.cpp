#include "QuadTre.h"

QuadTre::QuadTre()
{
	this->A = Point2F{ -2.5f, -2.5f };
	this->B = Point2F{ 2.5f, -2.5f };
	this->C = Point2F{ 2.5f, 2.5f };
	this->D = Point2F{ -2.5f, 2.5f };
	this->isLeaf = isLeaf;

	this->so = nullptr;
	this->sv = nullptr;
	this->no = nullptr;
	this->nv = nullptr;
}

QuadTre::QuadTre(Point2F &a, Point2F &b, Point2F &c, Point2F &d, bool isLeaf)
{
	this->A = a;
	this->B = b;
	this->C = c;
	this->D = d;
	this->isLeaf = isLeaf;

	this->so = nullptr;
	this->sv = nullptr;
	this->no = nullptr;
	this->nv = nullptr;
	this->quads = vector<QuadTre*>();
}

QuadTre::Point2F QuadTre::findMiddle(Point2F& a, Point2F& b)
{
	Point2F c;

	c.x = (a.x + b.x) / 2;
	c.z = (a.z + b.z) / 2;

	return c;
}



void QuadTre::subDivide(int n)
{
	if (n > 0) {
		Point2F A = findMiddle(this->A, this->B);
		Point2F B = findMiddle(this->B, this->C);
		Point2F C = findMiddle(this->C, this->D);
		Point2F D = findMiddle(this->D, this->A);
		Point2F M = findMiddle(this->A, this->C);

		this->sv = new QuadTre(this->A, A, M, D, n == 1);
		this->sv->parent = this;
		this->quads.push_back(sv);
		this->sv->subDivide(n - 1);
		this->so = new QuadTre(A, this->B, B, M, n == 1);
		this->so->parent = this;
		this->quads.push_back(so);
		this->so->subDivide(n - 1);
		this->no = new QuadTre(M, B, this->C, C, n == 1);
		this->no->parent = this;
		this->quads.push_back(no);
		this->no->subDivide(n - 1);
		this->nv = new QuadTre(D, M, C, this->D, n == 1);
		this->nv->parent = this;
		this->quads.push_back(nv);
		this->nv->subDivide(n - 1);
	}
}

void QuadTre::print()
{
}

QuadTre* QuadTre::insert(PhysicsObject* object)
{
	if (this->isLeaf) {
		if (this->parent->parent == nullptr) {
			this->parent->objects.push_back(object);
		}
		else {
			this->parent->parent->objects.push_back(object);
		}
		this->objects.push_back(object);
		return this;
	}
	else {
		Point2F M = findMiddle(this->A, this->C);
		if (object->currPos.z < M.z) {
			if (object->currPos.x < M.x) {
				this->sv->insert(object);
			}
			else {
				this->so->insert(object);
			}
		}
		else {
			if (object->currPos.x < M.x) {
				this->nv->insert(object);
			}
			else {
				this->no->insert(object);
			}
		}
	}
}

QuadTre* QuadTre::find(Point2F& point)
{
	if (this->isLeaf) {
		return this;
	}
	else {
		Point2F M = findMiddle(this->A, this->C);
		if (point.z < M.z) {
			if (point.x < M.x) {
				sv->find(point);
			}
			else {
				so->find(point);
			}
		}
		else {
			if (point.x < M.x) {
				nv->find(point);
			}
			else {
				no->find(point);
			}
		}
	}
}

vector<PhysicsObject*> QuadTre::getObjects()
{
	return this->objects;
}

void QuadTre::clearObjects()
{
	if (!this->isLeaf) {
		for (QuadTre* child : this->quads) {
			child->clearObjects();
		}
	}
	this->objects.clear();
}

bool QuadTre::hasObjects()
{
	return !this->objects.empty();
}

//bool QuadTre::isObjectsWithinBounds(PhysicsObject* ball)
//{
//	Point2F A{ this->B.x - this->A.x, this->B.z - this->A.z };
//	Point2F B{ this->B.x - this->A.x, this->B.z - this->A.z };
//	Point2F C{ this->B.x - this->A.x, this->B.z - this->A.z };
//	Point2F D{ this->B.x - this->A.x, this->B.z - this->A.z };
//
//	return (ball->currPos.x - this->findMiddle(this->A, this->B).x + ball->currPos.z - this->findMiddle(this->A, this->B).z) > 0;
//}
