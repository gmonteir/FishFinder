#version 330 core
layout(location = 0) in vec3 vertPos;

out vec2 texCoord;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

uniform float time;

void main()
{
    gl_Position = vec4(vertPos, 1);
	texCoord = (vertPos.xy+vec2(1, 1))/2.0;

    // from https://learnopengl.com/In-Practice/2D-Game/Postprocessing
	if (chaos)
    {
        texCoord = vec2(0.2 * sin((time + texCoord.x) * 5) + texCoord.x, 
                        0.2 * cos((time + texCoord.y) * 5) + texCoord.y);
    }
    if (confuse)
    {
        float speed = 5;
        float stretch = 10;
        float rotation = 0.1; // also affects the stretch
        texCoord = (texCoord - vec2(0.5)) * 0.9 + vec2(0.5);
        float x = rotation * sin(time * speed + texCoord.x * stretch) + texCoord.x;
        float y = rotation * cos(time * speed + texCoord.y * stretch) + texCoord.y;
        texCoord = vec2(x, y);
    }

    if (shake)
    {
        float speed = 50;
        float strength = 0.05;
        gl_Position.x += cos(time * speed) * strength;        
        gl_Position.y += cos(time * speed / 2) * strength;        
    }
}
