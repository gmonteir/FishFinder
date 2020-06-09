#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D texBuf;
uniform sampler2D depthBuf;

void main(){
	vec3 texColor = texture( texBuf, texCoord ).rgb;
	vec3 depth = texture( depthBuf, texCoord ).rgb;
	vec3 fogColor = vec3(0.1, 0.12, 0.14);

	
	float fogDensity = 5;
	const float LOG2 = 1.442695;
	float z = 1-depth.r;
	//float fogFactor = exp2( -fogDensity * fogDensity * z * z * LOG2 );
	float fogFactor = exp2( -pow(fogDensity * max(z - 0.1, 0), 2) * LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	color = vec4(mix(fogColor, texColor, fogFactor), 1.0);
	//color = vec4(vec3(fogFactor), 1.0);
}
