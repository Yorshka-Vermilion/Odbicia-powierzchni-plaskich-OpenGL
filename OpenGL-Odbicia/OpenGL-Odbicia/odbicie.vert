#version 440

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aNormal;

out vec2 texCoord;
out vec4 vs_position;
out vec3 reflectedVector;
out vec3 normal;
uniform mat4 cameraMatrix;
uniform mat4 ModelMatrix;

uniform vec3 cameraPosition;

void main(){
  // Output position of the vertex, in clip space : MVP * position

  vs_position = ModelMatrix * vec4(aPos,1.f);
  texCoord = vec2(aTex.x, aTex.y*-1.f);
  gl_Position =  cameraMatrix*ModelMatrix*vec4(aPos, 1.0);
  
  vec3 viewVector = normalize(vs_position.xyz - vec3(cameraPosition.x,cameraPosition.y,cameraPosition.z));
  reflectedVector = reflect(viewVector, aNormal);

}