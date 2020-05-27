#include "Shapes.h"
#include "Constants.h"

using namespace glm;

Shapes::Shapes()
{
	addShape(RESOURCE_DIR + "/cube.obj", CUBE_SHAPE);
	addShape(RESOURCE_DIR + "/sphere.obj", SPHERE_SHAPE);
	addShape(RESOURCE_DIR + "/dory.obj", DORY_SHAPE);
	addShape(RESOURCE_DIR + "/nemo.obj", NEMO_SHAPE);
	addShape(RESOURCE_DIR + "/squirt.obj", SQUIRT_SHAPE);
	addShape(RESOURCE_DIR + "/jenny.obj", JENNY_SHAPE);
	addShape(RESOURCE_DIR + "/charlie.obj", CHARLIE_SHAPE);
	addShape(RESOURCE_DIR + "/bloat.obj", BLOAT_SHAPE);
	addShape(RESOURCE_DIR + "/marlin.obj", MARLIN_SHAPE);
	addShape(RESOURCE_DIR + "/gurgle.obj", GURGLE_SHAPE);
	addShape(RESOURCE_DIR + "/tree_coral.obj", TREE_CORAL_SHAPE);
	addShape(RESOURCE_DIR + "/soft_coral.obj", SOFT_CORAL_SHAPE);
	addShape(RESOURCE_DIR + "/elkhorn_coral.obj", ELKHORN_CORAL_SHAPE);
	addShape(RESOURCE_DIR + "/virus.obj", ENEMY_SHAPE);

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

shared_ptr<vector<shared_ptr<Shape>>> Shapes::getShape(const string& key)
{
	return allShapes[key];
}

float Shapes::getRotationOffset(const string& key) const
{
	if (key == DORY_SHAPE)
		return radians(90.0f);
	if (key == SQUIRT_SHAPE)
		return radians(-90.0f);
	return 0;
}