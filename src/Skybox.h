
#include "MatrixStack.h"

#include <vector>
#include <string>
#include <memory>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Skybox
{
	static const std::vector<std::string> faces; 

	Skybox();

public:
	static Skybox& getInstance();

	~Skybox() {}
	void draw(std::shared_ptr<MatrixStack>& M, glm::vec3 position);

private:
	// texture for skymap
	unsigned int cubeMapTexture;

	unsigned int createSky(const std::string& dir, const std::vector<std::string> faces);

};
