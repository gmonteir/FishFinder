#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D texBuf;
uniform sampler2D depthBuf;

void main(){
	vec3 texColor = texture( texBuf, texCoord ).rgb;
	vec3 depth = texture( depthBuf, texCoord ).rgb;
	vec3 fogColor = vec3(0.1, 0.15, 0.2);

	
	float fogDensity = 0.2;
	const float LOG2 = 1.442695;
	float z = depth.r; //gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = exp2( -fogDensity * fogDensity * z * z * LOG2 );
	//float fogFactor = exp2( -fogDensity * z );
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	color = vec4(mix(fogColor, texColor, fogFactor), 1.0);
	color = vec4(vec3(texColor + fogDensity * fogColor * fogFactor), 1.0);
	

	//if (depth > 0.5)
	//	color = vec4(mix(fogColor, vec3(texColor, 1.0), depth.r), 1.0);
	//else
	//	color = vec4(texColor, 1.0);
	//color = vec4(depth, 1.0);
	
}
