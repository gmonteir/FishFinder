#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;

uniform float fTime;
uniform vec3 targetPos;

void main(){
	vec3 blue = vec3(0.1, 0.1, 0.2);
	vec3 yellow = vec3(0.2, 0.2, 0.0);

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += blue;

	float x = pow((gl_FragCoord.x * sin(fTime)) / 600, 2);
	float y = pow((gl_FragCoord.y * cos(fTime / 2)) / 400, 2);
	texColor += yellow * sin(fTime + sin(fTime / 200 + x - y));

	/* modifying color based on side of the screen
	vec3 blue = vec3(0, 0, 0.5);
	vec3 orange = vec3(0.3, 0, 0.3);

	vec3 texColor = texture( texBuf, vec2(texCoord.x + fTime, texCoord.y + 0.2f * sin(gl_FragCoord.x / 100)) ).rgb;
	//TODO modify to show this is a 2D image
	if (gl_FragCoord.x > 400)
		texColor -= blue;
	if (gl_FragCoord.x < 800)
		texColor -= orange;
	*/

	/* orbitting light
	vec3 yellow = vec3(0.8, 0.8, 0.0);
	vec2 orbitPoint = vec2(640, 480);
	float dist = distance(gl_FragCoord.xy, vec2(orbitPoint.x + 200 * sin(fTime), orbitPoint.y + 200 * cos(fTime)));

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += (1.0 - dist / 400) * yellow;
	*/
	color = vec4(texColor, 1.0);

}
