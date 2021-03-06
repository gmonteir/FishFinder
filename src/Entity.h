#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Constants.h"
#include "Transform.h"
#include "Model.h"
#include "Behavior.h"
#include "Floor.h"

#include <memory>
#include <vector>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity
{
public:
	Entity(const std::string& shapeName, int behavior=Behavior::NONE)
		: transform(), model(shapeName), 
		behavior(Behavior::createBehavior(behavior, transform, model))
	{
		this->behavior->start();
	}
	~Entity() {}

	void update(float deltaTime);
	void draw(std::shared_ptr<MatrixStack> &M) const { model.draw(M, transform); }
	void draw(std::shared_ptr<Program>& prog, std::shared_ptr<MatrixStack>& M) const
		{ model.draw(prog, M, transform); }

	bool hasCollided(Entity &entity);
	bool hasCollided(std::vector<std::shared_ptr<Entity>>& collectionEntities);

	float distance(const Entity& entity) const 
		{ return glm::distance(transform.getPosition(), entity.transform.getPosition()); }

	// Getters
	Transform& getTransform() { return transform; }
	Model& getModel() { return model; }
	std::shared_ptr<Behavior> &getBehavior() { return behavior; }

	float getRadius() const { return glm::length(model.getScaledMax() * transform.getSize()); }
	/* max shifted by transform and model */
	glm::vec3 getMaxBoundCoordinate() const 
		{ return model.getScaledMax() * transform.getSize() + transform.getPosition(); }
	/* min shifted by transform and model */
	glm::vec3 getMinBoundCoordinate() const 
		{ return model.getScaledMin() * transform.getSize() + transform.getPosition(); }

	bool shouldRemove() { return behavior->removed(); }

	void bringToFloor(float offset=0) { behavior->bringToFloor(offset); }

protected:
	Transform transform;
	Model model;
	std::shared_ptr<Behavior> behavior;

private:

	void move(glm::vec3 delta, bool* outOfBounds, bool* inFloor);

	bool isOutOfBounds() const 
		{ return !isInside(transform.getPosition(), glm::vec3(WORLD_SIZE), glm::vec3(-WORLD_SIZE)); }
	bool isInFloor() 
		{ return !Floor::getInstance()->isAboveFloor(getMinBoundCoordinate(), getMaxBoundCoordinate()); }

	// checks if point is inside the bounding box defined by max and min
	static bool isInside(glm::vec3 pt, glm::vec3 max, glm::vec3 min) {
		return min.x < pt.x&& pt.x < max.x
			&& min.y < pt.y&& pt.y < max.y
			&& min.z < pt.z&& pt.z < max.z;
	}
};

#endif