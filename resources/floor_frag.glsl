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
uniform float time;

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos);
bool draw(float z_line);

void main()
{
	vec3 normal = normalize(fragNor);
    vec3 result = vec3(0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, fragPos);    
    
    color = vec4(result, 1.0);
    
    /* Light line */
    vec2 targetDir = normalize(targetPos.xz - eye.xz);
    vec2 fragDir = normalize(fragPos.xz - eye.xz);
    float d = dot(targetDir, fragDir);
    float dist = distance(eye.xz, fragPos.xz);
    if (d > 0.8 && dist < 40)
    {
       float increment = (1-(dist/40))*0.7*(d-0.8)/0.2;
       color += vec4(vec3(0, increment, increment), 0);

       /* animating */
       float radius = mod(floor(time*30), 40);
       float dist2 = distance(fragPos.xz, eye.xz); // distance from eye
       if (dist2 < radius+1 && dist2 > radius-1)
          color -= vec4(vec3(increment, increment/2, increment/2), color.w);
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