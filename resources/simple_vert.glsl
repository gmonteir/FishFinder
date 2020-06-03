#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout (location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 eye;
uniform mat4 LP;
uniform mat4 LV;
uniform float time;

out vec3 fragPos;
out vec3 fragNor;
out vec3 viewer;
out vec3 lightfPos;

void main()
{
	vertTex.xy; // ignore texture coordinates for meshes where I use materials
	vec3 inc = vec3(0.25*sin(7*time), 0.25*sin(5*time), 0.25*sin(2*time));
	gl_Position = P * V * M * vec4(vertPos + inc, 1.0);
	fragPos = (M * vec4(vertPos, 1.0)).xyz;
	fragNor = (M * vec4(vertNor, 0.0)).xyz;
	viewer = (eye - (M * vec4(vertPos, 1.0)).xyz);
	lightfPos = (LP * LV * M * vec4(vertPos, 1.0)).xyz;
}
