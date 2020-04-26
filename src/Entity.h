#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"
#include "Shape.h"
#include "Constants.h"
#include "ShaderManager.h"

#include <vector>
#include <functional>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity
{
public:
	/* shape, position, velocity, size, facing, material */
	Entity(std::vector<std::shared_ptr<Shape>>& shapes, glm::vec3 position = ORIGIN, glm::vec3 velocity = ORIGIN,
		glm::vec3 size = glm::vec3(1.0), glm::vec3 facing = -ZAXIS, int material = DEFAULT_MATERIAL, string tag = "DEFAULT", int program = SIMPLEPROG)
		: shapes(shapes), position(position), velocity(velocity), size(size), facing(facing), material(material), tag(tag),
		program(program), isDead(false), texture(nullptr), toRemove(false)

	{
		extractMinMax();
	}
	virtual ~Entity() {}

	void randomRespawn();

	virtual void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);
	virtual void draw(std::shared_ptr<MatrixStack> &M);

	virtual void onOutOfBounds(float deltaTime);
	bool isOutOfBounds() const 
		{ return !isInside(position, glm::vec3(WORLD_SIZE), glm::vec3(-WORLD_SIZE)); }

	virtual void onCollision(Entity& collider) {} // Note: called twice per collision check, once for this and once for the collider
	bool hasCollided(Entity &entity) const;
	bool hasCollided(std::vector<std::shared_ptr<Entity>> &entities);

	void stop() { velocity = ORIGIN; material = STOPPED_MATERIAL; }
	glm::vec3 getVelocity() const { return velocity; }
	glm::vec3 getShift() const { return shift; }
	glm::vec3 getMaxBoundCoordinate() const { return max * scale * size + position; } // note: max already shifted
	glm::vec3 getMinBoundCoordinate() const { return min * scale * size + position; } // note: min already shifted
	bool isAlive() { return !isDead; }
	void kill() { isDead = true; }
	bool shouldRemove() { return toRemove; }
	void remove() { toRemove = true; }

	void setPosition(glm::vec3 pos) { this->position = pos; }
	void setVelocity(glm::vec3 vel) { this->velocity = vel; }
	void setSize(glm::vec3 size) { this->size = size; }
	void setTag(std::string tag) { this->tag = tag; }
	void setMaterial(int material) { this->material = material; }
	std::string getTag() { return this->tag; }

	void setTexture(std::shared_ptr<Texture>& tex) { texture = tex; }

	// checks if point is inside the bounding box defined by max and min
	static bool isInside(glm::vec3 pt, glm::vec3 max, glm::vec3 min) { 
		return min.x < pt.x && pt.x < max.x
			&& min.y < pt.y && pt.y < max.y
			&& min.z < pt.z && pt.z < max.z;
	}

protected:
	glm::vec3 position, velocity, size, facing;
	bool isDead;
	bool toRemove;
	string tag;

	std::vector<std::shared_ptr<Shape>> shapes;
	std::shared_ptr<Texture> texture;
	int material, program;
	glm::vec3 max, min, shift, scale;
private:

	void extractMinMax();
	void extractShiftScale();
	
};

