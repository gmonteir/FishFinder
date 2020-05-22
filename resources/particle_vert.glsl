#version 330 core
layout(location = 0) in vec3 vertPos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float time;

void main()
{
	gl_PointSize = 5.0;
	gl_Position = P * V * M * vec4(vertPos + sin(time), 1);
}
