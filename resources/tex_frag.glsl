#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 vTexCoord;
in vec3 light;
uniform sampler2D Texture0;

out vec4 color;

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
    vec3 ambient = vec3(texture(Texture0, vTexCoord));
    vec3 diffuse = vec3(texture(Texture0, vTexCoord));
    // vec3 specular = spec * MatSpec;
    ambient *= attenuation;
    diffuse *= attenuation;
    // specular *= attenuation;
    
    color = vec4(ambient + diffuse, 1.0);
}

