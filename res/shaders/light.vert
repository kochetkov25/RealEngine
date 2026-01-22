#version 440

#include "cameraUniform.glsl"

layout(location = 0) in vec3 vertex_position;

uniform mat4 modelMatrix;

void main() 
{
   gl_Position = camera_u.projMat * camera_u.viewMat * modelMatrix * vec4(vertex_position, 1.0);
}