#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;

uniform float fTime;

uniform float offset[5] = float[]( 0.0, 1.0, 2.0, 3.0, 4.0 );
uniform float weight[5] = float[]( 0.2270270270, 0.1945945946, 0.1216216216,
 0.0540540541, 0.0162162162 );

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

	/*
	vec3 yellow = vec3(0.8, 0.8, 0.0);
	vec2 orbitPoint = vec2(640, 480);
	float dist = distance(gl_FragCoord.xy, vec2(orbitPoint.x + 200 * sin(fTime), orbitPoint.y + 200 * cos(fTime)));

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	texColor += (1.0 - dist / 400) * yellow;
	
	color = vec4(texColor, 1.0);
	*/

	vec3 texColor = texture( texBuf, texCoord ).rgb;
	color = vec4(texColor*weight[0], 1.0);
	for (int i=1; i <5; i ++) {
		color += vec4(texture( texBuf, texCoord + vec2(offset[i], 0.0)/512.0 ).rgb, 1)*weight[i];
		color += vec4(texture( texBuf, texCoord - vec2(offset[i], 0.0)/512.0 ).rgb, 1)*weight[i];
	}

}
