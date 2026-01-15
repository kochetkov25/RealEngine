#version 440

#include "cameraUniform.glsl"

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 texture_normals;

out vec2 texCoords;
out vec3 fragNormal;          // World-space normal
out vec3 fragTangent;         // World-space tangent (for normal mapping)
out vec3 fragBitangent;       // World-space bitangent (for normal mapping)
out vec3 vertexPosWorld;

uniform mat4 modelMatrix;
uniform bool useNormalMapping = false;  // Flag to enable/disable normal mapping

void main() 
{
   texCoords = texture_coords;

   // Calculate TBN matrix for normal mapping (if tangents/bitangents are available)
   // For now, we'll compute it from normals if needed
   mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
   
   // Transform normals to world space
   fragNormal = normalize(normalMatrix * texture_normals);
   
   // For normal mapping, we need tangent and bitangent
   // Since we don't have them yet, we'll derive a perpendicular vector
   // In a full implementation, these would come from vertex attributes
   if (useNormalMapping) {
      // Compute tangent and bitangent from normal
      vec3 tangent = normalize(normalMatrix * vec3(1.0, 0.0, 0.0));
      vec3 bitangent = normalize(cross(fragNormal, tangent));
      tangent = normalize(cross(bitangent, fragNormal));
      
      fragTangent = tangent;
      fragBitangent = bitangent;
   }

   // Vertex position in world space
   vertexPosWorld = (modelMatrix * vec4(vertex_position, 1.0)).xyz;

   // Final position in clip space
   gl_Position = camera_u.projMat * camera_u.viewMat * modelMatrix * vec4(vertex_position, 1.0);
}