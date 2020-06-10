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
	Model(const std::string& shapeName)
		: shapes(*Shapes::getInstance()->getShape(shapeName)), 
		animator(Animator::createAnimator(shapeName)),
		rotationOffset(Shapes::getInstance()->getRotationOffset(shapeName)),
		material(NO_MATERIAL), matProgram(SIMPLEPROG),
		texProgram(TEXTUREPROG), texture(nullptr), hasTexture(false)
	{
		extractMinMax();
		extractShiftScale();
	}
	~Model() {}

	void draw(std::shared_ptr<MatrixStack> &M, const Transform& transform) const;
	void draw(std::shared_ptr<Program>& prog, 
		std::shared_ptr<MatrixStack>& M, const Transform& transform) const;

	glm::vec3 getScaledMin() const { return min * scale; }
	glm::vec3 getScaledMax() const { return max * scale; }
	glm::vec3 getScaledSize() const { return (max - min) * scale; }
	glm::vec3 getShift() const { return shift; }
	glm::vec3 getScale() const { return scale; }
	const std::vector<std::shared_ptr<Shape>>& getShapes() const { return shapes; }
	Animator& getAnimator() { return *animator; }

	Model& setMaterial(int material) { this->material = material; return *this; }
	Model& setTexture(const std::string& textureName)
		{ texture = Textures::getInstance()->getTexture(textureName); enableTexture(); return *this; }
	Model& setMaterialProgram(int prog) { matProgram = prog; return *this; }
	Model& setTextureProgram(int prog) { texProgram = prog; return *this; }

	Model& disableTexture() { hasTexture = false; return *this; }
	Model& enableTexture() { hasTexture = true; return *this; }

private:
	const std::vector<std::shared_ptr<Shape>>& shapes;
	std::shared_ptr<Texture> texture;
	const std::unique_ptr<Animator> animator;
	float rotationOffset;

	bool hasTexture;
	int material, matProgram, texProgram;
	glm::vec3 max, min, shift, scale;

	void extractMinMax();
	void extractShiftScale();
	
};

