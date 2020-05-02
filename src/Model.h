#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"
#include "Textures.h"
#include "Shape.h"
#include "Shapes.h"
#include "Constants.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Animator.h"

#include <vector>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
public:
	/* shape, position, velocity, size, facing, material */
	Model(const std::string shapeName)
		: shapes(*Shapes::getInstance()->getShape(shapeName)), 
		animator(Animator::createAnimator(shapeName)),
		material(DEFAULT_MATERIAL), program(SIMPLEPROG), texture(nullptr)
	{
		extractMinMax();
		extractShiftScale();
	}
	~Model() {}

	virtual void draw(std::shared_ptr<MatrixStack> &M, const Transform& transform) const;

	glm::vec3 getScaledMin() const { return min * scale; }
	glm::vec3 getScaledMax() const { return max * scale; }
	glm::vec3 getScaledSize() const { return (max - min) * scale; }
	glm::vec3 getShift() const { return shift; }
	glm::vec3 getScale() const { return scale; }
	const std::vector<std::shared_ptr<Shape>>& getShapes() const { return shapes; }
	Animator& getAnimator() { return *animator; }

	void setMaterial(int material) { this->material = material; }
	void setTexture(std::shared_ptr<Texture>& tex) { texture = tex; }
	void setTexture(const std::string textureName) 
		{ texture = Textures::getInstance()->getTexture(textureName); }
	void setProgram(int prog) { program = prog; }

private:
	const std::vector<std::shared_ptr<Shape>>& shapes;
	std::shared_ptr<Texture> texture;
	const unique_ptr<Animator> animator;

	int material, program;
	glm::vec3 max, min, shift, scale;

	void extractMinMax();
	void extractShiftScale();
	
};

