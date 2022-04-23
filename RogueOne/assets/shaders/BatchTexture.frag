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

    //this version has artifacting issues when rendering non-white textures on AMD GPUs
    //help here https://discord.com/channels/349857236656324608/496257118098292736/700159380028391465
    //the solution seems bad? so I'm gonna hold off on it for now
    /*
        if (v_TextureIndex == 0.0)
        {
            color = texture(u_Textures[int(v_TextureIndex)], v_TextureCoords * v_TilingFactor) * v_Color;
        }
        else if (v_TextureIndex == 1.0)
        {
            color = texture(u_Textures[int(v_TextureIndex)], v_TextureCoords * v_TilingFactor) * v_Color;
        }
        else
        {
            color = texture(u_Textures[int(v_TextureIndex)], v_TextureCoords * v_TilingFactor) * v_Color;
        }
    */

    fragColor = texture(textures[int(vData.TexIndex)], vData.TexCoords * vData.Tiling) * vData.Color;
}  