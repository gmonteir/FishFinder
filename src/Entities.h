#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Entity.h"

#include <vector>

class Entities : public std::vector<std::shared_ptr<Entity>>
{
public:
	static std::shared_ptr<Entities> getInstance();

	Entities() : numActive(0) {}
	~Entities() {}
	
	void update(float deltaTime);
	void draw(std::shared_ptr<MatrixStack>& M);
	int getNumActive() { return numActive; }
	void incrementNumActive() { numActive++; }
	void decrementNumActive() { numActive--; }

private:
	int numActive;
};

