#version 330 core

in vec3 fFragPos;
in vec2 fTexCoords;
in vec3 fNormal;

out vec4 fFragColor;

void main()
{
    fFragColor = vec4(1.0f, 1.0f 1.0f, 1.0f); //texture(texture_diffuse1, fTexCoords);
}