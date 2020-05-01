#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"
#include "Shape.h"
#include "Constants.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Model.h"

#include <vector>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity
{
public:
	Entity(std::vector<std::shared_ptr<Shape>>& shapes)
		: transform(), model(shapes), tag("DEFAULT"), isDead(false), toRemove(false) {}
	virtual ~Entity() {}

	virtual void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);
	virtual void draw(std::shared_ptr<MatrixStack> &M);

	virtual void onOutOfBounds(float deltaTime);
	bool isOutOfBounds() const 
		{ return !isInside(transform.getPosition(), glm::vec3(WORLD_SIZE), glm::vec3(-WORLD_SIZE)); }

	virtual void onCollision(Entity& collider) {} // Note: called twice per collision check, once for this and once for the collider
	bool hasCollided(Entity &entity) const;
	bool hasCollided(std::vector<std::shared_ptr<Entity>> &entities);

	// Getters
	Transform& getTransform() { return transform; }
	Model& getModel() { return model; }
	std::string getTag() const { return this->tag; }

	glm::vec3 getMaxBoundCoordinate() const { return model.getScaledMax() * transform.getSize() + transform.getPosition(); } // note: max already shifted
	glm::vec3 getMinBoundCoordinate() const { return model.getScaledMin() * transform.getSize() + transform.getPosition(); } // note: min already shifted

	// Setters
	void setTag(std::string tag) { this->tag = tag; }
	void stop() { transform.setVelocity(ORIGIN); model.setMaterial(STOPPED_MATERIAL); }
	void remove() { toRemove = true; }

	void bringToFloor() {
		transform.setPosition(glm::vec3(
			transform.getPosition().x,
			transform.getSize().y * model.getScaledSize().y / 2 + FLOOR_POSITION.y + 0.2,
			transform.getPosition().z)
		);
	}

	// Conditions
	bool isAlive() { return !isDead; }
	void kill() { isDead = true; }
	bool shouldRemove() { return toRemove; }

	// checks if point is inside the bounding box defined by max and min
	static bool isInside(glm::vec3 pt, glm::vec3 max, glm::vec3 min) { 
		return min.x < pt.x && pt.x < max.x
			&& min.y < pt.y && pt.y < max.y
			&& min.z < pt.z && pt.z < max.z;
	}

protected:
	Transform transform;
	Model model;
	bool isDead;
	bool toRemove;
	string tag;
};

