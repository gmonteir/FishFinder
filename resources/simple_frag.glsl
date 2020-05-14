#version 330 core 
in vec3 fragPos;
in vec3 fragNor;
in vec3 viewer;
in vec3 light;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;
uniform vec3 lightCol;

out vec4 color;

void main()
{
	vec3 lightDir = normalize(light - fragPos);
    vec3 normal = normalize(fragNor);
    vec3 viewDir = normalize(viewer);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shine);

    // attenuation
    float distance = length(light - fragPos);
    float attenuation = 1.0 / (1.0 + 0.007 * distance + 0.0002 * (distance * distance));

    // combine results
    vec3 ambient = MatAmb;
    vec3 diffuse = diff * MatDif;
    vec3 specular = spec * MatSpec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    color = vec4(ambient + diffuse + specular, 1.0);
}
