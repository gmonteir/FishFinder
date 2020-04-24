#version 330 core 
in vec3 fragNor;
in vec3 WPos;
//to send the color to a frame buffer
layout(location = 0) out vec4 color;

uniform vec3 MatAmb;
uniform vec3 MatDif;

void main()
{
	MatAmb;
	MatDif;
	float epsilon = 0.2;
	vec3 LPos = vec3(2, 2, 2);
	vec3 vVec = - WPos;
	vec4 Dcolor;
    vec3 Dlight = normalize(LPos - WPos);
	vec3 normal = normalize(fragNor);

	float dCo = max(dot(normalize(Dlight), normal), 0);
	if (dot(normalize(vVec), normal) < epsilon) 
		Dcolor = vec4(vec3(1, 0.08, 0.58), 0.5); // if dot product of view and frag normal is perpendicualr, color edge hot pink
	else // dCo > 0.25
	{
		// ceil(dCo*3)/3: is either 1/3 (black), 2/3 (grey), or 3/3 (light grey)
		vec3 temp = 0.4*vec3(0.7)*ceil(dCo*3)/3 + vec3(1, 0.08, 0.58)*dCo;
		Dcolor = vec4(temp, 1); // else use dCo [0-1.0] to bin the color 
	}
	color = Dcolor;
}
