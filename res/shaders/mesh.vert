#version 440

#include "cameraUniform.glsl"

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 texture_normals;

#ifdef SKINNED
layout(location = 3) in uvec4 bone_ids;
layout(location = 4) in vec4  bone_weights;

#include "boneUniform.glsl"
#endif

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

    vec4 localPosition = vec4(vertex_position, 1.0);
    vec3 localNormal   = texture_normals;

#ifdef SKINNED
    mat4 boneTransform = bone_weights.x * u_FinalBones[bone_ids.x] +
                         bone_weights.y * u_FinalBones[bone_ids.y] +
                         bone_weights.z * u_FinalBones[bone_ids.z] +
                         bone_weights.w * u_FinalBones[bone_ids.w];

    localPosition = boneTransform * localPosition;
    localNormal   = mat3(boneTransform) * localNormal; 
#endif

    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    
    fragNormal = normalize(normalMatrix * localNormal);
    
    if (useNormalMapping) {
       vec3 tangent = normalize(normalMatrix * vec3(1.0, 0.0, 0.0));
       vec3 bitangent = normalize(cross(fragNormal, tangent));
       tangent = normalize(cross(bitangent, fragNormal));
       
       fragTangent = tangent;
       fragBitangent = bitangent;
    }

    vertexPosWorld = (modelMatrix * localPosition).xyz;

    gl_Position = camera_u.projMat * camera_u.viewMat * modelMatrix * localPosition;
}