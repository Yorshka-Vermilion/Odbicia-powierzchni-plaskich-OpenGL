#version 440
out vec4 color;

in vec2 texCoord;
uniform sampler2D tex0;
uniform samplerCube cubemap;

in vec3 reflectedVector;

void main(){
  color = texture(cubemap, vec3(reflectedVector.x, reflectedVector.y,-reflectedVector.z));
}