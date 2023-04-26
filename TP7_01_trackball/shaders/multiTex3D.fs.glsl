#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
out vec4 fFragColor;

void main() {
  fFragColor = texture(uTexture1, vTexCoords) + texture(uTexture2, vTexCoords);
}
