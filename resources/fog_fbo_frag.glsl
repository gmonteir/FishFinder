#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;

void main(){
	vec3 texColor = texture( texBuf, texCoord ).rgb;

	if (gl_FragDepth > 30)
		texColor = vec3(0);

	color = vec4(texColor, 1.0);

}
