#version 330 core 
out vec4 color;

uniform float time;
in float num;
void main()
{
    vec2 dToCenter = gl_PointCoord - vec2(0.5);
    if(length(dToCenter) > 0.4)
        discard;

    float d = (num+0.1)/0.2;
    float decider = mod(floor(100*num), 3);
    if (decider == 0)
    	color = vec4(vec3(1-d, d, 1-d), 1);
    else if (decider == 1)
    	color = vec4(vec3(d, 1-d, d), 1);
    else
    	color = vec4(vec3(d, 1-d, 0), 1);
}