#version 440
out vec4 color;

in vec2 texCoord;
uniform sampler2D tex0;

void main(){
  color = texture(tex0, texCoord);
}