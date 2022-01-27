#version 130

in vec2 sentPosition;
in vec4 sentColor;
in vec2 sentUV;

out vec4 color;

uniform sampler2D spriteTex;

void main() {

    vec4 textureColor = texture(spriteTex, sentUV);
    color = sentColor * textureColor;
}