#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
  

uniform mat4 cameraMatrix;


void main(){
  // Output position of the vertex, in clip space : MVP * position
  gl_Position =  cameraMatrix*vec4(aPos, 1.0);
}