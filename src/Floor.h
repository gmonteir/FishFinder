#pragma once

#ifndef FLOOR_INCLUDED
#define FLOOR_INCLUDED

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Constants.h"
#include "Shape.h"
#include "Shapes.h"
#include "Textures.h"
#include "stb_image.h"
#include "ShaderManager.h"
#include "MatrixStack.h"

using namespace std;

class Floor {

	Floor();

public:

	static shared_ptr<Floor> getInstance();
	void draw(shared_ptr<MatrixStack>& M) const;
	float getHeight(float x, float z) const;
	bool isAboveFloor(const glm::vec3 min, const glm::vec3 max) const;

private:
	float terrain[MAP_X][MAP_Z];
};

#endif