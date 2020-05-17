#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 fragNor;
in vec3 viewer;

uniform samplerCube skybox;

void main() {
	vec3 viewDir = -normalize(viewer); // fragPos - eye
	vec3 reflDir = reflect(viewDir, normalize(fragNor)); // reflect around frag normal
	FragColor = vec4(0.9*texture(skybox, reflDir).rgb, 1); // sample skybox with reflection vector
}
