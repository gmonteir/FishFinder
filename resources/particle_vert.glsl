#version 330 core
layout(location = 0) in vec3 vertPos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float time;

void main()
{
	gl_PointSize = 6.0;
	float inc = mod(floor(50*time), 100)/20.f;

	vec3 s = vertPos + vec3(0.5*sin(time), inc, 0);
	gl_Position = P * V * M * vec4(s, 0.1);
}
