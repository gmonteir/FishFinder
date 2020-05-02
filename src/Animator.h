#pragma once

#include "Shape.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Constants.h"

#include <vector>

class Animator
{
public:
	class NoAnimator;
	class DoryAnimator;

	Animator() : animationSpeed(1) {}
	virtual ~Animator() {}

	virtual void animate(float deltaTime) = 0;
	virtual void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog, 
		const std::vector<std::shared_ptr<Shape>>& shapes) const = 0;
	void setAnimationSpeed(float speed) { animationSpeed = speed; }

	static void animatePart(float dt, float* angle, bool* movingRight, float low, float high);
	static float calculateShift(float minCoord, float maxCoord);
	static void setupPart(const std::vector<std::shared_ptr<Shape>>& shapes,
		std::shared_ptr<MatrixStack> Model, int i, int pivot, const glm::vec3& angle);

	static std::unique_ptr<Animator> createAnimator(const std::string& modelName);

private:
	float animationSpeed;
};

class Animator::NoAnimator : public Animator
{
public:
	NoAnimator() : Animator() {}
	virtual ~NoAnimator() {}

	void animate(float deltaTime) override {}
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog, 
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;
};

class Animator::DoryAnimator : public Animator
{
public:
	DoryAnimator() : Animator(), leftFinRight(false), rightFinRight(false),
		tailRight(false), tail(ORIGIN), rightFin(ORIGIN), leftFin(ORIGIN) {}
	virtual ~DoryAnimator() {}

	void animate(float deltaTime) override;
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog,
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;

private:
	glm::vec3 leftFin, rightFin, tail;
	bool leftFinRight, rightFinRight, tailRight;
};
