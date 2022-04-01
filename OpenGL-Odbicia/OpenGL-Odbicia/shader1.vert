#version 440

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
  
layout(location = 2) in vec2 aTex;

out vec2 texCoord;
out vec3 vs_position;

uniform mat4 cameraMatrix;
uniform mat4 ModelMatrix;

void main(){
  // Output position of the vertex, in clip space : MVP * position

  vs_position = vec4(ModelMatrix * vec4(aPos,1.f)).xyz;
  texCoord = vec2(aTex.x, aTex.y*-1.f);
  gl_Position =  cameraMatrix*ModelMatrix*vec4(aPos, 1.0);
  
}