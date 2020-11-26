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

vec4 shadeDirectionalLight()
{
    // Ambient shading component
    vec3 ambient = texture(texture_diffuse1, fTexCoords).rgb * directionalLight.color * directionalLight.ambient;

    // Diffuse shading component
    vec3 normal = normalize(fNormal);
    float diff = max(dot(normal, directionalLight.direction), 0.0f);
    vec3 diffuse = diff * texture(texture_diffuse1, fTexCoords).rgb * directionalLight.color * directionalLight.diffuse;

    // Specular shading component
    vec3 viewDir = normalize(viewPos - fFragPos);
    vec3 reflectDir = reflect(-directionalLight.direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = spec * directionalLight.color * directionalLight.specular;

    return vec4(ambient + diffuse + specular, 1.0f);
}

vec4 shadePointLights()
{
    // Ambient shading component
    vec3 ambient = texture(texture_diffuse1, fTexCoords).rgb * pointLight.color * pointLight.ambient;

    // Diffuse shading component
    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(pointLight.position - fFragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * texture(texture_diffuse1, fTexCoords).rgb * pointLight.color * pointLight.diffuse;

    // Specular shading component
    vec3 viewDir = normalize(viewPos - fFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64);
    vec3 specular = spec * pointLight.color * pointLight.specular;

    float distance = length(pointLight.position - fFragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    return vec4(ambient * attenuation + diffuse * attenuation + specular * 0.5 * attenuation, 1.0f);
}

void main()
{

    //vec4 directionalColor = shadeDirectionalLight();
    vec4 pointColor = shadePointLights();

    fFragColor = pointColor; //texture(texture_diffuse1, fTexCoords);
}