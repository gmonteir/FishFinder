#version 330 core

//to send the color to a frame buffer
out vec4 color;

/* frame buffer to write out only depths*/
void main()
{
	float z = 1.0 - (gl_FragCoord.z / gl_FragCoord.w) / 10000.0;
	color = vec4(z, z, z, 1.0);

}
