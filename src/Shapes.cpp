#include "Shapes.h"

using namespace glm;

shared_ptr<Shapes> Shapes::getInstance()
{
	static shared_ptr<Shapes> instance(new Shapes);
	return instance;
}

void Shapes::addShape(const string& filename, const string& key)
{
	shared_ptr<vector<shared_ptr<Shape>>> shapes(new vector<shared_ptr<Shape>>);
	vec3 sMin, sMax;
	Shape::loadMultipleShapeMesh(shapes, &sMin, &sMax, filename);
	allShapes[key] = shapes;
}

shared_ptr<vector<shared_ptr<Shape>>> Shapes::getShape(const string& key)
{
	return allShapes[key];
}