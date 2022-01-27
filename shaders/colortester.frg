#version 130

in vec4 sentColor;
in vec2 sentPosition;
in vec2 sentUV;

//sprite texture
uniform sampler2D spriteTex;

out vec4 color;

void main()
{
	vec4 mappedTex = texture(spriteTex, sentUV);
	color = mappedTex;
}