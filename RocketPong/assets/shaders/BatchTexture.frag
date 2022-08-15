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
    vec4 texColor = vData.Color;

    //this version has artifacting issues when rendering non-white textures on AMD GPUs
    //help here https://discord.com/channels/349857236656324608/496257118098292736/700159380028391465

	switch(int(vData.TexIndex))
	{
		case  0: texColor *= texture(textures[ 0], vData.TexCoords * vData.Tiling); break;
		case  1: texColor *= texture(textures[ 1], vData.TexCoords * vData.Tiling); break;
		case  2: texColor *= texture(textures[ 2], vData.TexCoords * vData.Tiling); break;
		case  3: texColor *= texture(textures[ 3], vData.TexCoords * vData.Tiling); break;
		case  4: texColor *= texture(textures[ 4], vData.TexCoords * vData.Tiling); break;
		case  5: texColor *= texture(textures[ 5], vData.TexCoords * vData.Tiling); break;
		case  6: texColor *= texture(textures[ 6], vData.TexCoords * vData.Tiling); break;
		case  7: texColor *= texture(textures[ 7], vData.TexCoords * vData.Tiling); break;
		case  8: texColor *= texture(textures[ 8], vData.TexCoords * vData.Tiling); break;
		case  9: texColor *= texture(textures[ 9], vData.TexCoords * vData.Tiling); break;
		case 10: texColor *= texture(textures[10], vData.TexCoords * vData.Tiling); break;
		case 11: texColor *= texture(textures[11], vData.TexCoords * vData.Tiling); break;
		case 12: texColor *= texture(textures[12], vData.TexCoords * vData.Tiling); break;
		case 13: texColor *= texture(textures[13], vData.TexCoords * vData.Tiling); break;
		case 14: texColor *= texture(textures[14], vData.TexCoords * vData.Tiling); break;
		case 15: texColor *= texture(textures[15], vData.TexCoords * vData.Tiling); break;
		case 16: texColor *= texture(textures[16], vData.TexCoords * vData.Tiling); break;
		case 17: texColor *= texture(textures[17], vData.TexCoords * vData.Tiling); break;
		case 18: texColor *= texture(textures[18], vData.TexCoords * vData.Tiling); break;
		case 19: texColor *= texture(textures[19], vData.TexCoords * vData.Tiling); break;
		case 20: texColor *= texture(textures[20], vData.TexCoords * vData.Tiling); break;
		case 21: texColor *= texture(textures[21], vData.TexCoords * vData.Tiling); break;
		case 22: texColor *= texture(textures[22], vData.TexCoords * vData.Tiling); break;
		case 23: texColor *= texture(textures[23], vData.TexCoords * vData.Tiling); break;
		case 24: texColor *= texture(textures[24], vData.TexCoords * vData.Tiling); break;
		case 25: texColor *= texture(textures[25], vData.TexCoords * vData.Tiling); break;
		case 26: texColor *= texture(textures[26], vData.TexCoords * vData.Tiling); break;
		case 27: texColor *= texture(textures[27], vData.TexCoords * vData.Tiling); break;
		case 28: texColor *= texture(textures[28], vData.TexCoords * vData.Tiling); break;
		case 29: texColor *= texture(textures[29], vData.TexCoords * vData.Tiling); break;
		case 30: texColor *= texture(textures[30], vData.TexCoords * vData.Tiling); break;
		case 31: texColor *= texture(textures[31], vData.TexCoords * vData.Tiling); break;
	}
	
    fragColor = texColor;
}  