#version 330 core
layout(location = 0) in vec3 vertPos;

out vec2 fragPos;
//uniform mat4 P;
//uniform mat4 V;

void main()
{
	gl_Position = vec4(vertPos, 0);
	fragPos = vertPos.xy;
	//fragPos = (vertPos.xy + vec2(1.0, 1.0)) / 2.0;
}
