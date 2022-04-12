#version 330 core
layout(location = 0) in vec3 position; 
layout(location = 1) in vec4 color; 
layout(location = 2) in vec2 texCoords;
layout(location = 3) in float texIndex;
layout(location = 4) in float tiling;

uniform mat4 projection;

out struct VertexOutputData
{
    vec4 Color;
    vec2 TexCoords;
    float TexIndex;
    float Tiling;
} vData;

void main()
{
    vData.Color = color;
    vData.TexCoords = texCoords;
    vData.TexIndex = texIndex;
    vData.Tiling = tiling;
    gl_Position = projection * vec4(position, 1.0);
}