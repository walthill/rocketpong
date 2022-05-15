#version 330 core
layout(location = 0) in vec3 position; 
layout(location = 1) in vec4 a_color; 
layout(location = 2) in vec2 texCoords;

uniform mat4 projection;

out vec4 vColor;
out vec2 vTexCoords;

void main()
{
    vColor = a_color;
    vTexCoords = texCoords;
    gl_Position = projection * vec4(position, 1.0);
}