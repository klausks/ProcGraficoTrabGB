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
    float average = color.r * 0.2115 + color.g * 0.7154 + color.b * 0.0731;
    color = vec4(average, average, average, 1.0);
}