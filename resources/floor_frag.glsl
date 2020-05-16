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

    vec3 targetDir = normalize(targetPos - eye);
    vec2 fourth_between = eye.xz + (targetPos.xz-eye.xz)/4;
    float d = distance(fragPos.xz, fourth_between);
    if (d < 7) // first circle
    {
       float increment = 0.5*(1-d/7);
       color += vec4(vec3(0, increment, increment/2), 1);
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