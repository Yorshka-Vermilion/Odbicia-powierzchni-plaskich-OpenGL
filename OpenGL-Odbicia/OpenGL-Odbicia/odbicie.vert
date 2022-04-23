#version 440

layout(location = 0) in vec3 aPos;  

out vec4 position; 

uniform mat4 cameraMatrix;
uniform mat4 ModelMatrix;

void main(){
  position = cameraMatrix*ModelMatrix*vec4(aPos, 1.0);
  gl_Position =  position;
}