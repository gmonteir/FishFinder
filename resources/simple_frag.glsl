#version 330 core 
in vec3 fragPos;
in vec3 fragNor;
in vec3 viewer;
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
uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;
uniform sampler2D shadowDepth;

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 pos, vec3 view);
float TestShadow(vec3 lfPos);

void main()
{
	vec3 normal = normalize(fragNor);
    vec3 viewDir = normalize(viewer);
    vec3 result = vec3(0);

    result += CalcPointLight(pointLights[0], normal, fragPos, viewDir);
    result += CalcPointLight(pointLights[1], normal, fragPos, viewDir);
    result += CalcPointLight(pointLights[2], normal, fragPos, viewDir);

    float shade = TestShadow(lightfPos);

    result = (1.0 - shade) * result;
    
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

float TestShadow(vec3 lfPos)
{
    float bias = 0.005;
	//1: shift the coordinates from -1, 1 to 0 ,1
    vec3 shifted = 0.5 * (lfPos + vec3(1.0));
	//2: read off the stored depth (.) from the ShadowDepth, using the shifted.xy 
    vec4 Ld = texture(shadowDepth, shifted.xy);
	//3: compare to the current depth (.z) of the projected depth

	//4: return 1 if the point is shadowed
    if (Ld.x < shifted.z - bias) {
        return 1.0;
    }

	return 0.0;
}