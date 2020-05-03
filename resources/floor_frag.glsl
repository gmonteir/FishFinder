#version 330 core
uniform sampler2D Texture0;
uniform sampler2D Texture1;

in vec2 vTexCoord;
in float dCo;
out vec4 Outcolor;

void main() {
	//vec4 texColor0 = texture(Texture0, vTexCoord);
	// vec3 color = (dCo+0.3)* texColor0.xyz;
	//vec3 color = texColor0.xyz;
  	//Outcolor = vec4(color, texColor0.w);

  	Outcolor = mix(texture(Texture0, vTexCoord), texture(Texture1, vTexCoord), 0.5);
}

