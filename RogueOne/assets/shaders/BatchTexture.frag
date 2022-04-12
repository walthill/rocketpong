#version 330 core

in struct VertexOutputData
{
    vec4 Color;
    vec2 TexCoords;
    float TexIndex;
    float Tiling;
} vData;

uniform sampler2D textures[32];

out vec4 fragColor;
void main()
{   
    fragColor = texture(textures[int(vData.TexIndex)], vData.TexCoords * vData.Tiling) * vData.Color;
}  