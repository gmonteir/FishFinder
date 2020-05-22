#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, TexCoords); 
	float factor = (TexCoords.y + 1)/2;
	FragColor = vec4(factor*FragColor.rgb, 1);
}
