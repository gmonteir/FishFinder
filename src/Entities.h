#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Entity.h"

#include <vector>

class Entities : public std::vector<std::shared_ptr<Entity>>
{
public:
	static std::shared_ptr<Entities> getInstance();

	Entities() {}
	~Entities() {}
	
	void update(float deltaTime);
	void draw(std::shared_ptr<MatrixStack>& M);
};

