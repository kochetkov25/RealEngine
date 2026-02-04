#version 440

#include "cameraUniform.glsl"
#include "lightUniform.glsl"

out vec4 FragColor; 

in vec2 texCoords;
in vec3 fragNormal;
in vec3 fragTangent;
in vec3 fragBitangent;
in vec3 vertexPosWorld;

struct Material {
    sampler2D diffuse;      // Unit 0
    sampler2D specular;     // Unit 1
    sampler2D normal;       // Unit 2
    sampler2D height;       // Unit 3
    sampler2D ambient;      // Unit 4
    sampler2D emissive;     // Unit 5
    sampler2D metallic;     // Unit 6
    sampler2D roughness;    // Unit 7
    
    // Legacy texture names
    sampler2D texture;      // Legacy diffuse
    sampler2D specularMap;  // Legacy specular
    sampler2D emissionMap;  // Legacy emission
    
    float shininess;
};

uniform Material material;

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

// Fallbacks
uniform vec3 materialColor = vec3(1.0, 1.0, 1.0);
uniform bool useNormalMapping = false;


vec3 calculateNormal(vec3 vertexNormal) {
    if (!useNormalMapping || !hasMaterialNormal) {
        return normalize(vertexNormal);
    }
    
    vec3 normalMapSample = texture(material.normal, texCoords).rgb;
    vec3 tangentNormal = normalize(normalMapSample * 2.0 - 1.0);
    
    vec3 N = normalize(vertexNormal);
    vec3 T = normalize(fragTangent);
    // Gram-Schmidt process (re-orthogonalize)
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
}

void main() 
{    
    // Translate to Linear Space: pow(color, 2.2)
    vec3 albedo = pow(materialColor, vec3(2.2));
    
    if (hasMaterialDiffuse) {
        vec3 sRGB = texture(material.diffuse, texCoords).rgb;
        albedo = pow(sRGB, vec3(2.2));
    } else if (hasMaterialTexture) { // Legacy fallback
        vec3 sRGB = texture(material.texture, texCoords).rgb;
        albedo = pow(sRGB, vec3(2.2));
    }

    vec3 specMap = vec3(1.0);
    if (hasMaterialSpecular) {
        specMap = texture(material.specular, texCoords).rgb;
    } else if (hasMaterialSpecularMap) { // Legacy fallback
        specMap = texture(material.specularMap, texCoords).rgb;
    }
    
    // Emissive Map (gamma correction 2.2)
    vec3 emissive = vec3(0.0);
    if (hasMaterialEmissive) {
        vec3 sRGB = texture(material.emissive, texCoords).rgb;
        emissive = pow(sRGB, vec3(2.2));
    } else if (hasMaterialEmissionMap) { // Legacy fallback
        vec3 sRGB = texture(material.emissionMap, texCoords).rgb;
        emissive = pow(sRGB, vec3(2.2));
    }
    
    float ao = hasMaterialAmbient ? texture(material.ambient, texCoords).r : 1.0;
    
    vec3 N = calculateNormal(fragNormal);
    vec3 V = normalize(camera_u.position - vertexPosWorld);
    
    vec3 totalLighting = vec3(0.0);
    
    int activeLights = min(light_u._count, 100);

    for (int i = 0; i < activeLights; ++i) {
        vec4 posData = light_u._data[i].position;
        vec4 colData = light_u._data[i].color;
        vec4 attData = light_u._data[i].attenuation;
        vec4 facData = light_u._data[i].factors;

        vec3  lightPos   = posData.xyz;
        int   lightType  = int(posData.w); // 0 = Point, 1 = Directional
        vec3  lightColor = colData.rgb * colData.w; // Color * Intensity
        
        float attConst   = attData.x;
        float attLin     = attData.y;
        float attQuad    = attData.z;
        float radius     = attData.w;
        
        float fAmbient   = facData.x;
        float fDiffuse   = facData.y;
        float fSpecular  = facData.z;

        vec3 L;
        float attenuation = 1.0;

        if (lightType == 1) { 
            // --- DIRECTIONAL LIGHT ---
            L = normalize(lightPos); 
            attenuation = 1.0; 
        } else { 
            // --- POINT LIGHT ---
            vec3 diff = lightPos - vertexPosWorld;
            float dist = length(diff);
            
            if (dist > radius) continue;

            L = normalize(diff);
            
            float attBasic = 1.0 / (attConst + attLin * dist + attQuad * (dist * dist));
            
            float distByRadius = dist / radius;
            float window = pow(max(1.0 - pow(distByRadius, 4.0), 0.0), 2.0);
            
            attenuation = attBasic * window;
        }

        // --- Blinn-Phong Shading ---
        
        // Diffuse
        float NdotL = max(dot(N, L), 0.0);
        vec3 diffuseContrib = (lightColor * fDiffuse) * albedo * NdotL;

        // Specular
        vec3 H = normalize(L + V);
        float NdotH = max(dot(N, H), 0.0);
        float specPower = pow(NdotH, material.shininess);
        vec3 specularContrib = (lightColor * fSpecular) * specMap * specPower;

        // Ambient (Per-light ambient is uncommon, usually it's global, but keeping logic)
        vec3 ambientContrib = (lightColor * fAmbient) * albedo * ao;

        totalLighting += (ambientContrib + diffuseContrib + specularContrib) * attenuation;
    }

    totalLighting += emissive;

    vec3 mapped = totalLighting / (totalLighting + vec3(1.0));
    
    vec3 finalColor = pow(mapped, vec3(1.0 / 2.2));
    
    FragColor = vec4(finalColor, 1.0);
}