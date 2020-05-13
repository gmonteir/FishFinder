#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 vTexCoord;
in vec3 light;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

out vec4 color;

void main()
{
	vec3 lightDir = normalize(fragPos - light);
    vec3 normal = normalize(fragNor);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewer, reflectDir), 0.0), shine);

    // attenuation
    float distance = length(fragPos - light);
    float attenuation = 1.0 / (1.0 + 0.007 * distance + 0.0002 * (distance * distance));

    // combine results
    vec3 ambient = vec3(mix(texture(Texture0, vTexCoord), texture(Texture1, vTexCoord), 0.5));
    vec3 diffuse = vec3(mix(texture(Texture0, vTexCoord), texture(Texture1, vTexCoord), 0.5));
    // vec3 specular = spec * MatSpec;
    ambient *= attenuation;
    diffuse *= attenuation;
    // specular *= attenuation;
    
    color = vec4(ambient + diffuse, 1.0);
}

