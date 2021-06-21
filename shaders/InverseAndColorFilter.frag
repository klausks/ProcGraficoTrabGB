#version 450 core
in vec3 ourColor;
in vec2 TexCoord;
uniform vec3 colorFilter;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord); //* vec4(ourColor, 1.0);
    color = vec4(1 - color.r + colorFilter.r, 1 - color.g + colorFilter.g, 1 - color.b + colorFilter.b, 1.0);
}