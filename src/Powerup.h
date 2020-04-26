#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"
#include "Shape.h"
#include "Entity.h"
#include "Constants.h"
#include "ShaderManager.h"

#include <vector>
#include <functional>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Powerup : public Entity
{
public:
	Powerup(std::vector<std::shared_ptr<Shape>>& shapes) : Entity(shapes), timer(POWERUP_DESPAWN_TIME) { }
	void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

protected:

private:
	float timer;
	
};

