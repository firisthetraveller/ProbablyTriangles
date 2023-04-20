#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fFragColor;

void main() {
  fFragColor = vec4(normalize(vNormal), 1);
}
