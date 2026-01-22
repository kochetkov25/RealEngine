#version 440

#include "cameraUniform.glsl"

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 vertex_uv;

out vec2 texCoords;

uniform mat4 modelMatrix;
uniform vec2 spriteUVMin;
uniform vec2 spriteUVMax;

void main() 
{
    texCoords = mix(spriteUVMin, spriteUVMax, vertex_uv);
    vec4 worldPos = modelMatrix * vec4(vertex_position, 0.0, 1.0);
    gl_Position = camera_u.projMat * camera_u.viewMat * worldPos;
}