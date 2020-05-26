#version 330 core 

in vec3 fragPos;
in vec3 fragNor;

//to send the color to a frame buffer
out vec4 color;
uniform vec3 eye;

/* frame buffer to write out only depths*/
void main()
{
	// depth = z_buffer / MAP_SIZE
	//shift the normal as images 0-1 (reshift on read out)

	//float depth = fragPos.z / 256;
	//color = vec4(vec3(depth, 0, 0), 1.0);
	//color = vec4(vec3(distance(eye, fragPos)) / 256, 1.0);
	//if (gl_FragCoord.z > 1)
	//	color = vec4(vec3(0), 1.0);
	//else
	//	color = vec4(vec3(1), 1.0);
	//color = vec4(vec3(gl_FragCoord.z), 1.0);
	//color = vec4(vec3(gl_FragCoord.z / gl_FragCoord.w), 1.0);
	//color = vec4(//gl_FragCoord.x > 1 ? 0 : gl_FragCoord.x,
				 //gl_FragCoord.y > 1 ? 0 : gl_FragCoord.y,
				 //gl_FragCoord.z > 1 ? 0 : gl_FragCoord.z,
				 //gl_FragCoord.w > 1 ? 0 : gl_FragCoord.w
				 //);

	float z = 1.0 - (gl_FragCoord.z / gl_FragCoord.w) / 10000.0;
	color = vec4(z, z, z, 1.0);

	/*
	const float LOG2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = exp2( -0.5 * 
				   0.5 * 
				   z * 
				   z * 
				   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	color = mix(gl_Fog.color, finalColor, fogFactor );
	*/

}
