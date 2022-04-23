#version 440

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

out vec2 texCoord;

uniform mat4 cameraMatrix;
uniform mat4 ModelMatrix;

void main(){
  texCoord = vec2(aTex.x, aTex.y*-1.f);
  gl_Position =  cameraMatrix*ModelMatrix*vec4(aPos, 1.0);
}