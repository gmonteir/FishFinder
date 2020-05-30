#pragma once
#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <string>
#include <vector>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include "tiny_obj_loader.h"

class Program;

class Shape
{
public:
	Shape();
	~Shape();
	void createShape(tinyobj::shape_t & shape);
	void loadMesh(const std::string &meshName);
	void loadSingleShapeMesh(const std::string &meshName);
	void measure();
	void allocNorms();
	void resize();
	void init();
	void draw(const std::shared_ptr<Program> prog) const;
	glm::vec3 min;
	glm::vec3 max;

	static void loadMultipleShapeMesh(std::shared_ptr<std::vector<std::shared_ptr<Shape>>> shapes,
		glm::vec3* gMin, glm::vec3* gMax, const std::string& meshName);
	static void setBuffers(std::shared_ptr<std::vector<std::shared_ptr<Shape>>> shapes,
		std::vector<float>& posBuf,
		std::vector<float>& texBuf,
		std::vector<unsigned int>& eleBuf);
	
private:
	std::vector<unsigned int> eleBuf;
	std::vector<float> posBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;
	unsigned eleBufID;
	unsigned posBufID;
	unsigned norBufID;
	unsigned texBufID;
    unsigned vaoID;
};
void getShiftAndScale(glm::vec3 *shift, glm::vec3 *scale, glm::vec3 *gMin, glm::vec3 *gMax);
void updateBounds(int i, const std::shared_ptr<Shape> shape,
  				  glm::vec3 *gMin, glm::vec3 *gMax);

#endif
