#version 330 core 
out vec4 color;

void main()
{
    vec2 dToCenter = gl_PointCoord - vec2(0.5);
    if(length(dToCenter) > 0.4)
        discard;
    color = vec4(vec3(0.5, 0.5, 1), 0.5);
}