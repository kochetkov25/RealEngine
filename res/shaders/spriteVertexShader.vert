#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 texture_normals;

out vec2 texCoords;
out vec3 texNormals;
out vec3 vertexPosWorld;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() 
{
   texCoords  = texture_coords;

   // scale obj normals
   texNormals = mat3(transpose(inverse(modelMatrix))) * texture_normals;

   // vertex pos in world space (without clip and view space)
   vertexPosWorld = (modelMatrix * vec4(vertex_position, 1.0)).xyz;

   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
}