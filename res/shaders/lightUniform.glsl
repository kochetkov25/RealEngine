struct LightData {
    vec4 position;    // .w = Type (0 = Point, 1 = Directional)
    vec4 color;       // .w = Intensity
    vec4 attenuation; // .x = const, .y = lin, .z = quad, .w = radius
    vec4 factors;     // .x = ambient, .y = diffuse, .z = specular
};

layout(std140, binding = 1) uniform LightBlock {
    LightData _data[100];  
    int       _count;      
} light_u;