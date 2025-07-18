#version 440
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;

out vec4 vertexColour;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    vertexColour = colour;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}