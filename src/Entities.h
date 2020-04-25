#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Entity.h"

#include <iostream>
#include <vector>
#include <functional>

class Entities : public std::vector<std::shared_ptr<Entity>>
{
public:
	static std::shared_ptr<Entities> getInstance();

	Entities() {}
	~Entities() {}
	
	void update(float deltaTime);
	void draw(std::shared_ptr<Program>& prog, std::shared_ptr<MatrixStack>& M);
};

