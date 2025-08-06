#version 440

#include "cameraUniform.glsl"

#include "lightUniform.glsl"

out vec4 vertexColor;

in vec2 texCoords;
in vec3 texNormals;
in vec3 vertexPosWorld;

uniform sampler2D tex;

struct Material
{
   sampler2D texture;

   sampler2D specularMap;
   sampler2D emissionMap;

   float shininess;
};

uniform Material material;

void main() 
{
   vec3 baseColor = texture(material.texture, texCoords).rgb;
   vec3 specularTex = texture(material.specularMap, texCoords).rgb;
   vec3 emission = texture(material.emissionMap, texCoords).rgb;

   vec3 normal = normalize(texNormals);
   vec3 cameraDirection = normalize(camera_u.position - vertexPosWorld);

   vec3 ambient = vec3(0.0);
   vec3 diffuse = vec3(0.0);
   vec3 specular = vec3(0.0);

   for (int numLight = 0; numLight < light_u._count; ++numLight) {
      LightData lightData = light_u._data[numLight];

      // diffuse
      vec3 lightDirection = normalize(lightData.lightPosition - vertexPosWorld);
      float ndotl = max(dot(normal, lightDirection), 0.0);

      // specular
      vec3 reflectDirection = reflect(-lightDirection, normal);
      float specularDot = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shininess);

      // attenuation
      float dist = length(lightData.lightPosition - vertexPosWorld);
      float attenuation = 1.0 / (lightData.constantFactor + lightData.linearFactor * dist + lightData.quadFactor * (dist * dist));

      vec3 amb = lightData.ambientFactor * baseColor * lightData.lightColor;
      vec3 dif = lightData.diffuseFactor * baseColor * lightData.lightColor * ndotl;
      vec3 spe = specularTex * lightData.specularFactor * lightData.lightColor * specularDot;

      ambient  += amb * attenuation;
      diffuse  += dif * attenuation;
      specular += spe * attenuation;
   }

   vertexColor = vec4(ambient + diffuse + specular + emission, 1.0);
}