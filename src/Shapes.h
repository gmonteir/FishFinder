#pragma once

#ifndef SHAPES_INCLUDED
#define SHAPES_INCLUDED

#include "Shape.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Shapes {

	Shapes();

public:
	static std::shared_ptr<Shapes> getInstance();

	~Shapes() {}

	std::unordered_map<std::string, std::shared_ptr<std::vector<std::shared_ptr<Shape>>>> allShapes;
	void addShape(const std::string& filename, const std::string& key);
	void addShape(std::vector<float>& posBuf, std::vector<float>& texBuf, 
		std::vector<unsigned int>& eleBuf, const std::string& key);
	std::shared_ptr<std::vector<std::shared_ptr<Shape>>> getShape(const std::string& key);
	float getRotationOffset(const std::string& key) const;
};

#endif