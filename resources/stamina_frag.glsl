#version 330 core 

in vec2 fragPos;
out vec4 color;

uniform float stamina;

void main()
{
   // if (stamina < fragPos.x) {
      //  color = vec4(vec3(1, 0, 0), 1);
   // }
    //else {
        //color = vec4(stamina, stamina, stamina, 1);
    //}

    color = vec4(fragPos, 0, 1);
    //color = vec4(vec3(1, 0, 0), 1);
}