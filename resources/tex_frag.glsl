#version 330 core
uniform sampler2D Texture0;

in vec2 vTexCoord;
in float dCo;
in vec3 WPos;
out vec4 Outcolor;

void main() {
	vec4 texColor0 = texture(Texture0, vTexCoord);
	vec3 color = (dCo+0.3)* texColor0.xyz;
  	Outcolor = vec4(color, texColor0.w);
}

