#version 330 core

in vec3 fragPos;
in vec3 fragNor;
in vec2 vTexCoord;

out vec4 color;
uniform sampler2D texBuf;
uniform vec3 targetPos;

void main(){
	vec3 texColor = texture( texBuf, vTexCoord ).rgb;

	if (gl_FragCoord.z > 0.5)
		texColor = vec3(0);

	color = vec4(texColor, 1.0);
	color = vec4(vec3(distance(targetPos, fragPos)), 1.0);

}
