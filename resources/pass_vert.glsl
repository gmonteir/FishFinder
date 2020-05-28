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
        float strength = 0.3;
        texCoord = vec2(texCoord.x + sin(time) * strength, texCoord.y + cos(time) * strength);
    }
    else if (confuse)
    {
        texCoord = vec2(1.0 - texCoord.x, 1.0 - texCoord.y);
    }

    if (shake)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;        
        gl_Position.y += cos(time * 15) * strength;        
    }
}
