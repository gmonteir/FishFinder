#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"
#include "Shape.h"
#include "Constants.h"
#include "ShaderManager.h"
#include "Transform.h"

#include <vector>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
public:
	/* shape, position, velocity, size, facing, material */
	Model(std::vector<std::shared_ptr<Shape>>& shapes)
		: shapes(shapes), material(DEFAULT_MATERIAL), program(SIMPLEPROG), texture(nullptr)
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

	void setMaterial(int material) { this->material = material; }
	void setTexture(std::shared_ptr<Texture>& tex) { texture = tex; }

protected:
	const std::vector<std::shared_ptr<Shape>>& shapes;
	std::shared_ptr<Texture> texture;
	int material, program;
	glm::vec3 max, min, shift, scale;
private:

	void extractMinMax();
	void extractShiftScale();
	
};

