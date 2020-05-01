#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"
#include "Shape.h"
#include "Constants.h"
#include "ShaderManager.h"
#include "Transform.h"

#include <vector>
#include <functional>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity
{
public:
	/* shape, position, velocity, size, facing, material */
	Entity(std::vector<std::shared_ptr<Shape>>& shapes)
		: shapes(shapes), transform(), material(DEFAULT_MATERIAL), tag("DEFAULT"),
		program(SIMPLEPROG), isDead(false), texture(nullptr), toRemove(false)

	{
		extractMinMax();
	}
	virtual ~Entity() {}

	void randomRespawn();

	virtual void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);
	virtual void draw(std::shared_ptr<MatrixStack> &M);

	virtual void onOutOfBounds(float deltaTime);
	bool isOutOfBounds() const 
		{ return !isInside(transform.getPosition(), glm::vec3(WORLD_SIZE), glm::vec3(-WORLD_SIZE)); }

	virtual void onCollision(Entity& collider) {} // Note: called twice per collision check, once for this and once for the collider
	bool hasCollided(Entity &entity) const;
	bool hasCollided(std::vector<std::shared_ptr<Entity>> &entities);

	void stop() { transform.setVelocity(ORIGIN); material = STOPPED_MATERIAL; }
	Transform& getTransform() { return transform; }
	glm::vec3 getShift() const { return shift; }
	glm::vec3 getMaxBoundCoordinate() const { return max * scale * transform.getSize() + transform.getPosition(); } // note: max already shifted
	glm::vec3 getMinBoundCoordinate() const { return min * scale * transform.getSize() + transform.getPosition(); } // note: min already shifted
	bool isAlive() { return !isDead; }
	void kill() { isDead = true; }
	bool shouldRemove() { return toRemove; }
	void remove() { toRemove = true; }

	void bringToFloor() { transform.setPosition(glm::vec3(
		transform.getPosition().x, 
		transform.getSize().y * scale.y * (max.y - min.y) / 2 + FLOOR_POSITION.y + 0.2,
		transform.getPosition().z)); }
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
	Transform transform;
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

