#version 330 core 

in vec3 fragPos;
in vec3 fragNor;

//to send the color to a frame buffer
out vec4 color;

/* frame buffer to write out only depths*/
void main()
{
	// depth = z_buffer / MAP_SIZE
	float depth = fragPos.z / 256;
	//shift the normal as images 0-1 (reshift on read out)
	color = vec4(vec3(depth), 1.0);
	color = vec4(vec3(fragPos.z), 1.0);
}
