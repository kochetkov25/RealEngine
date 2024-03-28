#version 440
out vec4 vertexColor;

uniform vec3 lightColor;

void main() 
{
   vertexColor = vec4(lightColor, 1.f);
}