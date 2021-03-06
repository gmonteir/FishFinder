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
	class PlayerAnimator;
	class ClownfishAnimator;
	class TurtleAnimator;
	class MotherAnimator;
	class FatherAnimator;
	class PufferfishAnimator;
	class SharkAnimator;

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

class Animator::PlayerAnimator : public Animator
{
public:
	PlayerAnimator() : Animator(), leftFinRight(false), rightFinRight(false),
		tailRight(false), tail(ORIGIN), rightFin(ORIGIN), leftFin(ORIGIN) {}
	virtual ~PlayerAnimator() {}

	void animate(float deltaTime) override;
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog,
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;

private:
	glm::vec3 leftFin, rightFin, tail;
	bool leftFinRight, rightFinRight, tailRight;
};

class Animator::ClownfishAnimator : public Animator
{
public:
	ClownfishAnimator() : Animator(), leftFin(ORIGIN), rightFin(ORIGIN), tail(ORIGIN), leftFoot(ORIGIN), rightFoot(ORIGIN),
		leftFinRight(false), rightFinRight(false), tailRight(false), leftFootRight(false), rightFootRight(false) {}
	virtual ~ClownfishAnimator() {}

	void animate(float deltaTime) override;
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog,
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;

private:
	glm::vec3 leftFin, rightFin, tail, leftFoot, rightFoot;
	bool leftFinRight, rightFinRight, tailRight, leftFootRight, rightFootRight;
};

class Animator::TurtleAnimator : public Animator
{
public:
	TurtleAnimator() : Animator(), leftFin(ORIGIN), rightFin(ORIGIN), leftFoot(ORIGIN), rightFoot(ORIGIN),
		leftFinRight(false), rightFinRight(false), leftFootRight(false), rightFootRight(false) {}
	virtual ~TurtleAnimator() {}

	void animate(float deltaTime) override;
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog,
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;

private:
	glm::vec3 leftFin, rightFin, leftFoot, rightFoot;
	bool leftFinRight, rightFinRight, leftFootRight, rightFootRight;
};

class Animator::MotherAnimator : public Animator
{
public:
	MotherAnimator() : Animator(), leftFin(ORIGIN), rightFin(ORIGIN), tail(ORIGIN),
		leftFinRight(false), rightFinRight(false), tailRight(false) {}
	virtual ~MotherAnimator() {}

	void animate(float deltaTime) override;
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog,
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;

private:
	glm::vec3 leftFin, rightFin, tail;
	bool leftFinRight, rightFinRight, tailRight;
};

class Animator::FatherAnimator : public Animator
{
public:
	FatherAnimator() : Animator(), leftFin(ORIGIN), rightFin(ORIGIN), tail(ORIGIN),
		leftFinRight(false), rightFinRight(false), tailRight(false) {}
	virtual ~FatherAnimator() {}

	void animate(float deltaTime) override;
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog,
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;

private:
	glm::vec3 leftFin, rightFin, tail;
	bool leftFinRight, rightFinRight, tailRight;
};

class Animator::PufferfishAnimator : public Animator
{
public:
	PufferfishAnimator() : Animator(), leftFin(ORIGIN), rightFin(ORIGIN), tail(ORIGIN),
		leftFinRight(false), rightFinRight(false), tailRight(false) {}
	virtual ~PufferfishAnimator() {}

	void animate(float deltaTime) override;
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog,
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;

private:
	glm::vec3 leftFin, rightFin, tail;
	bool leftFinRight, rightFinRight, tailRight;
};

class Animator::SharkAnimator : public Animator
{
public:
	SharkAnimator() : Animator(), tail(ORIGIN), body(ORIGIN), tailRight(false), bodyRight(false) {}
	virtual ~SharkAnimator() {}

	void animate(float deltaTime) override;
	void drawModel(std::shared_ptr<MatrixStack>& M, std::shared_ptr<Program> prog,
		const std::vector<std::shared_ptr<Shape>>& shapes) const override;

private:
	glm::vec3 tail, body;
	bool tailRight, bodyRight;
};

