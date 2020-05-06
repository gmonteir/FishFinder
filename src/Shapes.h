#pragma once

#ifndef SHAPES_INCLUDED
#define SHAPES_INCLUDED

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Shape.h"

using namespace std;

class Shapes {

	Shapes();

public:
	static shared_ptr<Shapes> getInstance();

	~Shapes() {}

	unordered_map<string, shared_ptr<vector<shared_ptr<Shape>>>> allShapes;
	void addShape(const string& filename, const string& key);
	void addShape(vector<float>& posBuf, vector<float>& texBuf, vector<unsigned int>& eleBuf, const string& key);
	shared_ptr<vector<shared_ptr<Shape>>> getShape(const string& key);
	float getRotationOffset(const std::string& key) const;
};

#endif