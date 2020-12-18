#include "Shapes.h"
#include "Constants.h"

#include <iostream>

using namespace std;
using namespace glm;

Shapes::Shapes()
{
	addShape(RESOURCE_DIR + "/cube.obj", CUBE_SHAPE);
	addShape(RESOURCE_DIR + "/sphere.obj", SPHERE_SHAPE);
	addShape(RESOURCE_DIR + "/player.obj", PLAYER_SHAPE);
	addShape(RESOURCE_DIR + "/clown1.obj", CLOWN1_SHAPE);
	addShape(RESOURCE_DIR + "/turtle.obj", TURTLE_SHAPE);
	addShape(RESOURCE_DIR + "/mother.obj", MOTHER_SHAPE);
	addShape(RESOURCE_DIR + "/father.obj", FATHER_SHAPE);
	addShape(RESOURCE_DIR + "/puffer.obj", PUFFER_SHAPE);
	addShape(RESOURCE_DIR + "/clown2.obj", CLOWN2_SHAPE);
	addShape(RESOURCE_DIR + "/royal.obj", ROYAL_SHAPE);
	addShape(RESOURCE_DIR + "/shark.obj", SHARK_SHAPE);
	addShape(RESOURCE_DIR + "/tree_coral.obj", TREE_CORAL_SHAPE);
	addShape(RESOURCE_DIR + "/soft_coral.obj", SOFT_CORAL_SHAPE);
	addShape(RESOURCE_DIR + "/elkhorn_coral.obj", ELKHORN_CORAL_SHAPE);
	addShape(RESOURCE_DIR + "/virus.obj", STATIC_ENEMY_SHAPE);

	cout << "Shapes: Initialized" << endl;
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

void Shapes::addShape(vector<float>& posBuf,
	vector<float>& texBuf,
	vector<unsigned int>& eleBuf,
	const string& key)
{
	shared_ptr<vector<shared_ptr<Shape>>> shapes(new vector<shared_ptr<Shape>>);
	Shape::setBuffers(shapes, posBuf, texBuf, eleBuf);
	allShapes[key] = shapes;
}

float Shapes::getRotationOffset(const string& key) const
{
	if (key == PLAYER_SHAPE)
		return radians(90.0f);
	if (key == TURTLE_SHAPE)
		return radians(-90.0f);
	return 0;
}