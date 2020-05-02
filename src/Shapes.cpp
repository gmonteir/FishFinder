#include "Shapes.h"
#include "Constants.h"

using namespace glm;

Shapes::Shapes()
{
	addShape(RESOURCE_DIR + "/cube.obj", CUBE_SHAPE);
	addShape(RESOURCE_DIR + "/dory.obj", DORY_SHAPE);
	addShape(RESOURCE_DIR + "/nemo.obj", NEMO_SHAPE);
	addShape(RESOURCE_DIR + "/tree_coral.obj", TREE_CORAL_SHAPE);
	addShape(RESOURCE_DIR + "/soft_coral.obj", SOFT_CORAL_SHAPE);
	addShape(RESOURCE_DIR + "/elkhorn_coral.obj", ELKHORN_CORAL_SHAPE);
}

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

float Shapes::getRotationOffset(const string& key)
{
	if (key == DORY_SHAPE)
		return radians(80.0f);
	return 0;
}