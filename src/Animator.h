#pragma once

#include "Shape.h"
#include "Program.h"
#include "MatrixStack.h"

#include <vector>

class Animator
{
public:
	class NoAnimator;

	Animator() {}
	virtual ~Animator() {}

	virtual std::unique_ptr<Animator> clone() const = 0;
	virtual void animate(float deltaTime) = 0;

	static void animatePart(float dt, float* angle, bool* movingRight, float low, float high);
	static float calculateShift(float minCoord, float maxCoord);
	static void setupPart(const std::vector<std::shared_ptr<Shape>> shape,
		std::shared_ptr<MatrixStack> Model, int i, int pivot, glm::vec3* angle);

private:
	virtual void drawModel(std::shared_ptr<MatrixStack>& M, 
		std::shared_ptr<Program> prog, const std::vector<std::shared_ptr<Shape>>& shapes) const = 0;
};

class Animator::NoAnimator : Animator
{
	NoAnimator() {}
	virtual ~NoAnimator() {}

	std::unique_ptr<Animator> clone() const override 
		{ return std::unique_ptr<Animator>(new NoAnimator); }
	void animate(float deltaTime) override {}

private:
	void drawModel(std::shared_ptr<MatrixStack>& M, 
		std::shared_ptr<Program> prog, const std::vector<std::shared_ptr<Shape>>& shapes) const override;
};
