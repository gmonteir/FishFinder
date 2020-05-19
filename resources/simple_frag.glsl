#version 330 core 
in vec3 fragPos;
in vec3 fragNor;
in vec3 viewer;

out vec4 color;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  
};

#define NR_POINT_LIGHTS 3
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos, vec3 view);  

void main()
{
	vec3 normal = normalize(fragNor);
    vec3 viewDir = normalize(viewer);
    vec3 result = vec3(0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, fragPos, viewDir);
    
    color = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos, vec3 view)
{
    vec3 lightDir = normalize(light.position - pos);

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(view, reflectDir), 0.0), shine);

    // attenuation
    float distance = length(light.position - pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // combine results
    vec3 ambient = MatAmb;
    vec3 diffuse = diff * MatDif;
    vec3 specular = spec * MatSpec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
} 