#version 440

#include "cameraUniform.glsl"
#include "lightUniform.glsl"

out vec4 vertexColor;

in vec2 texCoords;
in vec3 fragNormal;
in vec3 fragTangent;
in vec3 fragBitangent;
in vec3 vertexPosWorld;

// Material struct matching C++ MaterialInfo uniform names
// Supports both new material system and legacy uniforms
struct Material {
    sampler2D diffuse;      // Unit 0: Diffuse/Albedo texture
    sampler2D specular;     // Unit 1: Specular map
    sampler2D normal;       // Unit 2: Normal map
    sampler2D height;       // Unit 3: Height/Displacement map
    sampler2D ambient;      // Unit 4: Ambient Occlusion map
    sampler2D emissive;     // Unit 5: Emissive map
    sampler2D metallic;     // Unit 6: Metallic map (PBR)
    sampler2D roughness;    // Unit 7: Roughness map (PBR)
    
    // Legacy texture names (backward compatibility)
    sampler2D texture;      // Legacy diffuse texture
    sampler2D specularMap;  // Legacy specular map
    sampler2D emissionMap;  // Legacy emission map
    
    float shininess;        // Material shininess/specular power
};

uniform Material material;

// Flags to indicate which textures are available (optional optimization)
uniform bool hasMaterialDiffuse = false;
uniform bool hasMaterialSpecular = false;
uniform bool hasMaterialNormal = false;
uniform bool hasMaterialHeight = false;
uniform bool hasMaterialAmbient = false;
uniform bool hasMaterialEmissive = false;
uniform bool hasMaterialMetallic = false;
uniform bool hasMaterialRoughness = false;
uniform bool hasMaterialTexture = false;      // Legacy
uniform bool hasMaterialSpecularMap = false;  // Legacy
uniform bool hasMaterialEmissionMap = false;  // Legacy

// Material color fallback (if no texture)
uniform vec3 materialColor = vec3(1.0, 1.0, 1.0);

// Normal mapping toggle
uniform bool useNormalMapping = false;

/**
 * Sample texture with fallback to default value
 * Uses sampler2D directly (GLSL will handle invalid samplers gracefully)
 */
vec3 sampleTextureSafe(sampler2D tex, vec3 defaultValue) {
    // Check if sampler is valid by sampling (invalid samplers return default)
    vec4 texSample = texture(tex, texCoords);
    // If all components are zero, likely unused sampler
    return (texSample.rgb == vec3(0.0) && texSample.a == 0.0) ? defaultValue : texSample.rgb;
}

/**
 * Sample texture with explicit flag check (more efficient)
 */
vec3 sampleTexture(sampler2D tex, bool hasTexture, vec3 defaultValue) {
    return hasTexture ? texture(tex, texCoords).rgb : defaultValue;
}

/**
 * Sample texture alpha with fallback
 */
float sampleTextureAlpha(sampler2D tex, bool hasTexture, float defaultValue) {
    return hasTexture ? texture(tex, texCoords).a : defaultValue;
}

/**
 * Calculate normal from normal map if available, otherwise use vertex normal
 */
vec3 calculateNormal(vec3 vertexNormal) {
    if (!useNormalMapping || !hasMaterialNormal) {
        return normalize(vertexNormal);
    }
    
    // Sample normal map (stored in tangent space, typically [0,1] range)
    vec3 normalMapSample = texture(material.normal, texCoords).rgb;
    
    // Convert from [0,1] to [-1,1] range
    vec3 tangentNormal = normalize(normalMapSample * 2.0 - 1.0);
    
    // Construct TBN matrix
    vec3 N = normalize(vertexNormal);
    vec3 T = normalize(fragTangent);
    vec3 B = normalize(fragBitangent);
    
    // Ensure T, B, N form an orthonormal basis
    T = normalize(T - dot(T, N) * N);
    B = cross(N, T);
    
    mat3 TBN = mat3(T, B, N);
    
    // Transform tangent-space normal to world space
    return normalize(TBN * tangentNormal);
}

void main() 
{
   // Sample base color (diffuse texture)
   // Priority: hasMaterialDiffuse -> hasMaterialTexture -> materialColor
   vec3 baseColor = materialColor;
   
   if (hasMaterialDiffuse) {
       // New material system - diffuse texture
       baseColor = texture(material.diffuse, texCoords).rgb;
   } else if (hasMaterialTexture) {
       // Legacy support - material.texture
       baseColor = texture(material.texture, texCoords).rgb;
   }
   // Otherwise use materialColor (fallback)

   // Sample specular map
   vec3 specularColor = vec3(1.0);
   if (hasMaterialSpecular) {
       specularColor = texture(material.specular, texCoords).rgb;
   } else if (hasMaterialSpecularMap) {
       // Legacy support
       specularColor = texture(material.specularMap, texCoords).rgb;
   }
   
   // Sample emissive map
   vec3 emissiveColor = vec3(0.0);
   if (hasMaterialEmissive) {
       emissiveColor = texture(material.emissive, texCoords).rgb;
   } else if (hasMaterialEmissionMap) {
       // Legacy support
       emissiveColor = texture(material.emissionMap, texCoords).rgb;
   }
   
   // Sample ambient occlusion (affects ambient lighting)
   float aoFactor = hasMaterialAmbient ? texture(material.ambient, texCoords).r : 1.0;
   
   // Calculate final normal (with normal mapping if enabled)
   vec3 normal = calculateNormal(fragNormal);
   
   // View direction
   vec3 viewDirection = normalize(camera_u.position - vertexPosWorld);

   // Accumulate lighting contributions
   vec3 ambient = vec3(0.0);
   vec3 diffuse = vec3(0.0);
   vec3 specular = vec3(0.0);

   // Process all lights
   for (int i = 0; i < light_u._count && i < 100; ++i) {
      LightData light = light_u._data[i];

      // Light direction
      vec3 lightDirection = normalize(light.lightPosition - vertexPosWorld);
      
      // Distance attenuation
      float dist = length(light.lightPosition - vertexPosWorld);
      float attenuation = 1.0 / (light.constantFactor + light.linearFactor * dist + 
                                  light.quadFactor * (dist * dist));

      // Diffuse contribution (Lambertian)
      float ndotl = max(dot(normal, lightDirection), 0.0);
      vec3 diffuseContrib = light.diffuseFactor * baseColor * light.lightColor * ndotl;

      // Specular contribution (Blinn-Phong for better performance than Phong)
      vec3 halfDirection = normalize(lightDirection + viewDirection);
      float specularDot = pow(max(dot(normal, halfDirection), 0.0), material.shininess);
      vec3 specularContrib = specularColor * light.specularFactor * light.lightColor * specularDot;

      // Ambient contribution (with AO factor)
      vec3 ambientContrib = light.ambientFactor * baseColor * light.lightColor * aoFactor;

      // Accumulate with attenuation
      ambient  += ambientContrib * attenuation;
      diffuse  += diffuseContrib * attenuation;
      specular += specularContrib * attenuation;
   }

   // Final color = ambient + diffuse + specular + emissive
   // Emissive is added without attenuation (self-illumination)
   vec3 finalColor = ambient + diffuse + specular + emissiveColor;
   
   vertexColor = vec4(finalColor, 1.0);
}