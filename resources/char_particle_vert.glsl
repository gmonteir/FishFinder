#version 330 core
layout(location = 0) in vec3 vertPos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float time;

out float num;
void main()
{
	gl_PointSize = 10.0;
	vec3 dir = normalize(vertPos);
	float inc = mod(floor(15*time), 100)/20.f;

	num = vertPos.x;
	vec3 s = vertPos + 0.8*sin(5*time)*dir;
	gl_Position = P * V * M * vec4(s, 0.1);
}
