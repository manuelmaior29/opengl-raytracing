#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 color;
out vec2 fTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    color = vec3(1.0f, 0.0f, 0.0f);

    fTexCoords = aTexCoords;

    gl_Position = projection * model * vec4(aPos, 1.0);
}