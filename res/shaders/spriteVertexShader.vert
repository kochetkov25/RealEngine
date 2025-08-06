#version 440

#include "cameraUniform.glsl"

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 texture_normals;

out vec2 texCoords;
out vec3 texNormals;
out vec3 vertexPosWorld;

uniform mat4 modelMatrix;

void main() 
{
   texCoords  = texture_coords;

   // scale obj normals
   texNormals = mat3(transpose(inverse(modelMatrix))) * texture_normals;

   // vertex pos in world space (without clip and view space)
   vertexPosWorld = (modelMatrix * vec4(vertex_position, 1.0)).xyz;

   gl_Position = camera_u.projMat * camera_u.viewMat * modelMatrix * vec4(vertex_position, 1.0);
}