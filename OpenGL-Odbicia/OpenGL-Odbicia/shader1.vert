#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
  
layout(location = 2) in vec2 aTex;

out vec2 texCoord;

uniform mat4 cameraMatrix;


void main(){
  // Output position of the vertex, in clip space : MVP * position
  gl_Position =  cameraMatrix*vec4(aPos, 1.0);
  texCoord = vec2(aTex.x, aTex.y*-1.f);
}