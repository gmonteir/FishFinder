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

	virtual void animate(float deltaTime) = 0;

	static void animatePart(float dt, float* angle, bool* movingRight, float low, float high);
	static float calculateShift(float minCoord, float maxCoord);
	static void setupPart(const std::vector<std::shared_ptr<Shape>> shape,
		std::shared_ptr<MatrixStack> Model, int i, int pivot, glm::vec3* angle);

	static std::unique_ptr<Animator> createAnimator(const std::string& modelName);

private:
	virtual void drawModel(std::shared_ptr<MatrixStack>& M, 
		std::shared_ptr<Program> prog, const std::vector<std::shared_ptr<Shape>>& shapes) const = 0;
};

class Animator::NoAnimator : public Animator
{
public:
	NoAnimator() {}
	virtual ~NoAnimator() {}

	void animate(float deltaTime) override {}

private:
	void drawModel(std::shared_ptr<MatrixStack>& M, 
		std::shared_ptr<Program> prog, const std::vector<std::shared_ptr<Shape>>& shapes) const override;
};
