#version 330 core

in vec3 color;
in vec2 fTexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, fTexCoords);
}