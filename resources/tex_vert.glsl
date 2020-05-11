#version  330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightDir;

out float dCo;
out vec2 vTexCoord;
out vec3 fragNor;
out vec3 WPos;

void main() {
  vec4 vPosition;

  /* First model transforms */
  gl_Position = P * V * M * vec4(vertPos.xyz, 1.0);
  WPos = (M*vec4(vertPos.xyz, 1.0)).xyz;
  fragNor = (M * vec4(vertNor, 0.0)).xyz;

  /* diffuse coefficient for a directional light */
  dCo = max(dot(normalize(fragNor), normalize(lightDir)), 0);

  /* pass through the texture coordinates to be interpolated */
  vTexCoord = vertTex;
}
