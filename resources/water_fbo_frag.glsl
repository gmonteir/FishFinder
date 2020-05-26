#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;

uniform float time;
uniform vec3 targetPos;

void main(){
	vec3 texColor = texture( texBuf, texCoord ).rgb;

	/*
	vec3 blue = vec3(0.01, 0.08, 0.2);
	vec3 yellow = vec3(0.15, 0.15, 0.01);

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += blue;

	float x = cos(gl_FragCoord.x / 800  + time / 2);
	float y = sin(gl_FragCoord.y / 700 + time);
	texColor += yellow * pow(sin(x - y), 2);
	*/

	// --------------- 3 waves ----------------- //
	/*
	vec3 blue = vec3(0.03, 0.05, 0.1);
	vec3 yellow = vec3(0.1, 0.1, 0.01);

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += blue;

	float x = gl_FragCoord.x / 800 * cos(time / 2);
	float y = gl_FragCoord.y / 700 * sin(time) + 3;
	texColor += yellow * sin(x - y);
	*/
	
	// --------------- 2 faded lights ----------------- //
	/* 
	vec3 blue = vec3(0.03, 0.05, 0.1);
	vec3 yellow = vec3(0.09, 0.09, 0.01);

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += blue;

	float x = gl_FragCoord.x / 800 * cos(time / 2);
	float y = gl_FragCoord.y / 700 * sin(time) + 3;
	texColor += yellow * sin(pow(x, 2) - y);
	*/
	
	// --------------- first draft of water ----------------- //
	/*
	vec3 blue = vec3(0.03, 0.05, 0.1);
	vec3 yellow = vec3(0.09, 0.09, 0.01);

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += blue;

	float x = (gl_FragCoord.x * cos(time / 2)) / 800;
	float y = (gl_FragCoord.y * sin(time)) / 700;
	texColor += yellow * sin(time + sin(time / 200 + pow(x, 2) - pow(y, 2)));
	*/
	
	
	// --------------- modifying color based on side of the screen ----------------- //
	/* 
	vec3 blue = vec3(0, 0, 0.5);
	vec3 orange = vec3(0.3, 0, 0.3);

	vec3 texColor = texture( texBuf, vec2(texCoord.x + time, texCoord.y + 0.2f * sin(gl_FragCoord.x / 100)) ).rgb;
	if (gl_FragCoord.x > 400)
		texColor -= blue;
	if (gl_FragCoord.x < 800)
		texColor -= orange;
	*/
	
	// --------------- orbitting light ----------------- //
	/* 
	vec3 yellow = vec3(0.8, 0.8, 0.0);
	vec2 orbitPoint = vec2(640, 480);
	float dist = distance(gl_FragCoord.xy, vec2(orbitPoint.x + 200 * sin(time), orbitPoint.y + 200 * cos(time)));

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += (1.0 - dist / 400) * yellow;
	*/


	color = vec4(texColor, 1.0);

}
