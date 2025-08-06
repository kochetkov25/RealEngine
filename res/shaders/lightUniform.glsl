struct LightData {
    vec3  lightPosition;  float _pad0;
    vec3  lightColor;     float _pad1;

    vec3  ambientFactor;  float _pad2;
    vec3  diffuseFactor;  float _pad3;
    vec3  specularFactor; float _pad4;

    float constantFactor;
    float linearFactor;
    float quadFactor;

    float _pad5;           
};

layout(std140, binding = 1) uniform LightBlock {
    LightData _data[100];  
    int       _count;      
} light_u;