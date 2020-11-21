#version 330 core

in vec3 fFragPos;
in vec2 fTexCoords;
in vec3 fNormal;

out vec4 fFragColor;

// Structure definitions
struct DirectionalLight_
{
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight_
{
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform DirectionalLight_ directionalLight;
uniform PointLight_ pointLight;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

uniform vec3 viewPos;

/*
uniform vec3 lightPos;
uniform vec3 lightColor;
*/

// Shading variables
float ambientStrength = 0.1f;
float specularStrength = 0.5f;

void main()
{
    // Ambient shading component
    vec3 ambient = ambientStrength * texture(texture_diffuse1, fTexCoords).rgb * directionalLight.color;

    // Diffuse shading component
    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - fFragPos);
    vec3 diffuse = max(dot(normal, directionalLight.direction), 0.0f) * texture(texture_diffuse1, fTexCoords).rgb * directionalLight.color;

    // Specular shading component
    vec3 viewDir = normalize(viewPos - fFragPos);
    vec3 reflectDir = reflect(-directionalLight.direction, normal);
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0f), 32) * specularStrength * directionalLight.color;

    vec4 color = vec4(ambient + diffuse + specular, 1);

    fFragColor = color; //texture(texture_diffuse1, fTexCoords);
}