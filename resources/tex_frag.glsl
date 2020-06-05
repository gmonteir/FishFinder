#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 vTexCoord;
in vec3 lightfPos;

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
uniform sampler2D shadowDepth;

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos);
float TestShadow(vec3 lfPos);

void main()
{
    vec3 normal = normalize(fragNor);
    vec3 result = vec3(0);

    result += CalcPointLight(pointLights[0], normal, fragPos);
    result += CalcPointLight(pointLights[1], normal, fragPos);
    result += CalcPointLight(pointLights[2], normal, fragPos);

    float shade = TestShadow(lightfPos);

    result = (1.0 - shade) * result;
    
    color = vec4(result, 1.0);
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
    vec3 diffuse = vec3(texture(Texture0, vTexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse);
}

float TestShadow(vec3 lfPos)
{
    float shadow = 0.0;
    float bias = 0.005;
    vec3 shifted = 0.5 * (lfPos + vec3(1.0));

    vec2 texelSize = 1.0 / textureSize(shadowDepth, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowDepth, shifted.xy + vec2(x, y) * texelSize).r; 
            shadow += shifted.z - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}