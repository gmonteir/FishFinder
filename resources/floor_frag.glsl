#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 vTexCoord;
in vec3 light;
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform vec3 targetPos;
uniform vec3 eye;

out vec4 color;

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

void main()
{
	vec3 lightDir = normalize(light - fragPos);
    vec3 normal = normalize(fragNor);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(eye, reflectDir), 0.0), shine);

    // attenuation
    float distance = length(light - fragPos);
    float attenuation = 1.0 / (1.0 + 0.007 * distance + 0.0002 * (distance * distance));

    // combine results
    vec3 ambient = vec3(mix(texture(Texture0, vTexCoord), texture(Texture1, vTexCoord), 0.5));
    vec3 diffuse = vec3(mix(texture(Texture0, vTexCoord), texture(Texture1, vTexCoord), 0.5));
    // vec3 specular = spec * MatSpec;
    ambient *= attenuation;
    diffuse *= attenuation;
    // specular *= attenuation;
    
    color = vec4(ambient + diffuse, 1.0);

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