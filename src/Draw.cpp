#include "Draw.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>



using namespace glm;
using namespace std;

// sets material for the meshes
void SetMaterial(const shared_ptr<Program> prog, int i) 
{
	switch (i) 
	{
		case 0: // black
		glUniform3f(prog->getUniform("MatAmb"), 0.02, 0.02, 0.02);
		glUniform3f(prog->getUniform("MatDif"), 0.1, 0.1, 0.1);
		glUniform3f(prog->getUniform("MatSpec"), 0.4, 0.4, 0.4);
		glUniform1f(prog->getUniform("shine"), 10.0);
		break;
		case 1: // flat grey
		glUniform3f(prog->getUniform("MatAmb"), 0.13, 0.13, 0.14);
		glUniform3f(prog->getUniform("MatDif"), 0.3, 0.3, 0.4);
		glUniform3f(prog->getUniform("MatSpec"), 0.3, 0.3, 0.4);
		glUniform1f(prog->getUniform("shine"), 4.0);
		break;
		case 2: //white
		glUniform3f(prog->getUniform("MatAmb"), 0.3, 0.3, 0.3);
		glUniform3f(prog->getUniform("MatDif"), 0.55, 0.55, 0.55);
		glUniform3f(prog->getUniform("MatSpec"), 0.7, 0.7, 0.7);
		glUniform1f(prog->getUniform("shine"), 32.0);
		break;
		case 3: // ruby
		glUniform3f(prog->getUniform("MatAmb"), 0.1745, 0.01175, 0.01175);
		glUniform3f(prog->getUniform("MatDif"), 0.61424, 0.04136, 0.04136);
		glUniform3f(prog->getUniform("MatSpec"), 0.727811, 0.62695, 0.62695);
		glUniform1f(prog->getUniform("shine"), 76.8);
		break;
		case 4: // green
		glUniform3f(prog->getUniform("MatAmb"), 0.1745, 0.01175, 0.01175);
		glUniform3f(prog->getUniform("MatDif"), 0.1, 0.6, 0.1);
		glUniform3f(prog->getUniform("MatSpec"), 0.727811, 0.62695, 0.62695);
		glUniform1f(prog->getUniform("shine"), 76.8);
		break;
		case 5: // blue
		glUniform3f(prog->getUniform("MatAmb"), 0.05, 0.05, 0.2);
		glUniform3f(prog->getUniform("MatDif"), 0.0, 0.1, 0.9);
		glUniform3f(prog->getUniform("MatSpec"), 0.3, 0.3, 0.4);
		glUniform1f(prog->getUniform("shine"), 20.);
		break;
		case 6: // jade
		glUniform3f(prog->getUniform("MatAmb"), 0.135, 0.2225, 0.1575);
		glUniform3f(prog->getUniform("MatDif"), 0.54, 0.89, 0.63);
		glUniform3f(prog->getUniform("MatSpec"), 0.316228, 0.316228, 0.316228);
		glUniform1f(prog->getUniform("shine"), 12.8);
		break;
		case 7: // violet
		glUniform3f(prog->getUniform("MatAmb"), 0.1745, 0.01175, 0.1745);
		glUniform3f(prog->getUniform("MatDif"), 0.61424, 0.04136, 0.61424);
		glUniform3f(prog->getUniform("MatSpec"), 0.727811, 0.62695, 0.727811);
		glUniform1f(prog->getUniform("shine"), 76.8);
		break;
		case 8: // pink
		glUniform3f(prog->getUniform("MatAmb"), 0.1745, 0.05175, 0.1045);
		glUniform3f(prog->getUniform("MatDif"), 0.61424, 0.24136, 0.41424);
		glUniform3f(prog->getUniform("MatSpec"), 0.727811, 0.22695, 0.47811);
		glUniform1f(prog->getUniform("shine"), 76.8);
		break;
	}
}