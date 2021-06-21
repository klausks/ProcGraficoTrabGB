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
    if(color.g > 0.5)
    {
        color.r = 1.0;
        color.g = 1.0;
        color.b = 1.0;
    }
    else
    {
        color.r = 0;
        color.g = 0;
        color.b = 0;
    }
}