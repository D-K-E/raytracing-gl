#version 430 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D myTexture;

void main() { FragColor = vec4(texture(myTexture, TexCoord)); }
