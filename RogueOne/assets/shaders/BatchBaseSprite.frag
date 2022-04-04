#version 330 core

in vec4 vColor;
in vec2 vTexCoords;

uniform sampler2D image;
uniform vec3 spriteColor;

out vec4 color;
void main()
{    
    color = vColor; //vec4(vColor.xyz, 1.0f);
}  