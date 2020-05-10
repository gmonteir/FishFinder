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
  		brightFactor = (WPos.z-z_line)/3; // between -1 and 1 w/ 0 as the center
      // Color it brightest in the center 
      Outcolor = vec4((1.9-abs(brightFactor))*Outcolor.xyzw, Outcolor.w);
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