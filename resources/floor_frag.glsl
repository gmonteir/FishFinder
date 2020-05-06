#version 330 core
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform vec3 targetPos;
uniform vec3 eye;

in vec2 vTexCoord;
in float dCo;
out vec4 Outcolor;
in vec3 WPos;

bool draw(float z_line);

void main() {
  	Outcolor = mix(texture(Texture0, vTexCoord), texture(Texture1, vTexCoord), 0.5);

  	float slope;
  	float brightFactor;
  	float z_difference;
  	if (targetPos.x - eye.x == 0)
  		slope = (targetPos.z - eye.z)/0.001;
  	else
  		slope = (targetPos.z - eye.z)/(targetPos.x - eye.x);
  	float z_line = (slope*(WPos.x - targetPos.x) + targetPos.z);
  	if (draw(z_line))
  	{
  		// WPos.z - z_line  is between -3 and 3 (0 is the center of the line)
  		//brightFactor = ((((WPos.z - z_line)+3)/6)+0.5)*1.5;
  		//Outcolor = vec4(brightFactor*Outcolor.x, brightFactor*Outcolor.y, brightFactor*Outcolor.z, Outcolor.w);
  		brightFactor = (WPos.z-z_line)+3;
  		if (brightFactor < 1)
  			Outcolor = vec4(1.5*Outcolor.x, 0.5*Outcolor.y, 0.5*Outcolor.z, Outcolor.w);
  		else if (brightFactor < 2)
  			Outcolor = vec4(1.5*Outcolor.x, Outcolor.y, 0.5*Outcolor.z, Outcolor.w);
  		else if (brightFactor < 3)
  			Outcolor = vec4(1.5*Outcolor.x, 1.5*Outcolor.y, 0.5*Outcolor.z, Outcolor.w);
  		else if (brightFactor < 4)
  			Outcolor = vec4(0.5*Outcolor.x, 1.5*Outcolor.y, 0.5*Outcolor.z, Outcolor.w);
  		else if (brightFactor < 5)
  			Outcolor = vec4(0.5*Outcolor.x, 0.5*Outcolor.y, 1.5*Outcolor.z, Outcolor.w);
  		else
  			Outcolor = vec4(1.5*Outcolor.x, 0.5*Outcolor.y, 1.5*Outcolor.z, Outcolor.w);
  		//Outcolor = vec4(1.4*Outcolor.xyz, Outcolor.w);
  	}
}

bool draw(float z_line)
{
	float small_x = min(targetPos.x, eye.x);
	float large_x = max(targetPos.x, eye.x);
	float small_z = min(targetPos.z, eye.z);
	float large_z = max(targetPos.z, eye.z);
	return (WPos.z  - z_line < 3 && WPos.z - z_line > -3 &&
			WPos.z <= large_z && WPos.z >= small_z &&
			WPos.x <= large_x && WPos.x >= small_x);
}