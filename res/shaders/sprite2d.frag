#version 440

out vec4 vertexColor;

in vec2 texCoords;

uniform sampler2D spriteTexture;
uniform vec3 spriteColor = vec3(1.0, 1.0, 1.0);
uniform bool hasTexture = true;

void main() 
{
    vec4 texSample = texture(spriteTexture, texCoords);
    
    if (texSample.a < 0.01) {
        discard;
    }
    
    vertexColor = vec4(texSample.rgb * spriteColor, texSample.a);
}