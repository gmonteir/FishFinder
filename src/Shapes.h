#pragma once

#ifndef SHAPES_INCLUDED
#define SHAPES_INCLUDED

#include "Shape.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Shapes
{
	typedef std::shared_ptr<std::vector<std::shared_ptr<Shape>>> Value;

	Shapes();

public:
	static std::shared_ptr<Shapes> getInstance();

	~Shapes() {}

	void addShape(const std::string& filename, const std::string& key);
	void addShape(std::vector<float>& posBuf, std::vector<float>& texBuf, 
		std::vector<unsigned int>& eleBuf, const std::string& key);
	Value getShape(const std::string& key)
		{ return allShapes[key]; }
	float getRotationOffset(const std::string& key) const;

private:
	std::unordered_map<std::string, Value> allShapes;
};

#endif