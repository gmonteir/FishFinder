#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 vTexCoord;
in vec3 light;

out vec4 color;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  
};

#define NR_POINT_LIGHTS 3
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform vec3 targetPos;
uniform vec3 eye;

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos);
bool draw(float z_line);

void main()
{
	vec3 normal = normalize(fragNor);
    vec3 result = vec3(0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, fragPos);    
    
    color = vec4(result, 1.0);

  	float slope;
  	float brightFactor;
  	float z_difference;
  	if (targetPos.x - eye.x == 0)
  		slope = (targetPos.z - eye.z)/0.001;
  	else
  		slope = (targetPos.z - eye.z)/(targetPos.x - eye.x);
  	float z_line = (slope*(fragPos.x - targetPos.x) + targetPos.z);
  	if (draw(z_line))
  	{
  		brightFactor = (fragPos.z-z_line)/3; // between -1 and 1 w/ 0 as the center
		// Color it brightest in the center 
		color = vec4((1.9-abs(brightFactor))*color.xyz, color.w);
  	}
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos)
{
    vec3 lightDir = normalize(light.position - pos);

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // attenuation
    float distance = length(light.position - pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // combine results
    vec3 ambient = vec3(0.1);
    vec3 diffuse = vec3(mix(texture(Texture0, vTexCoord), texture(Texture1, vTexCoord), 0.5));
    ambient *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse);
} 

bool draw(float z_line)
{
	float small_x = min(targetPos.x, eye.x);
	float large_x = max(targetPos.x, eye.x);
	float small_z = min(targetPos.z, eye.z);
	float large_z = max(targetPos.z, eye.z);
	return (fragPos.z - z_line < 3 && fragPos.z - z_line > -3 &&
			fragPos.z <= large_z && fragPos.z >= small_z &&
			fragPos.x <= large_x && fragPos.x >= small_x);
}