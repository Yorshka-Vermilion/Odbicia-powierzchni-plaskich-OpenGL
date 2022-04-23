#version 440
out vec4 color;

in vec4 position;
uniform sampler2D reflectionTexture;

void main(){
  vec2 coords = (position.xy/position.w)/2 +0.5;
  color = texture(reflectionTexture, vec2(coords.x, -coords.y));
}