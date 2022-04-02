#version 440
out vec4 color;


in vec4 clipSpace;
uniform sampler2D reflectionTexture;

void main(){
  vec2 ndc = (clipSpace.xy/clipSpace.w)/2 +0.5;
  color = texture(reflectionTexture, vec2(ndc.x, -ndc.y));
}