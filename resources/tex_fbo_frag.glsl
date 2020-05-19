#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;

uniform float fTime;

void main(){
	/*
	vec3 blue = vec3(0, 0, 0.5);
	vec3 orange = vec3(0.3, 0, 0.3);

	vec3 texColor = texture( texBuf, vec2(texCoord.x + fTime, texCoord.y + 0.2f * sin(gl_FragCoord.x / 100)) ).rgb;
	//TODO modify to show this is a 2D image
	if (gl_FragCoord.x > 400)
		texColor -= blue;
	if (gl_FragCoord.x < 800)
		texColor -= orange;
	*/

	vec3 yellow = vec3(0.8, 0.8, 0.0);
	vec2 orbitPoint = vec2(640, 480);
	float dist = distance(gl_FragCoord.xy, vec2(orbitPoint.x + 200 * sin(fTime), orbitPoint.y + 200 * cos(fTime)));

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += (1.0 - dist / 400) * yellow;

	color = vec4(texColor, 1.0);

}
