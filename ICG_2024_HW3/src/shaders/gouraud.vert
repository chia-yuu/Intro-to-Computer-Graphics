#version 330 core

// TODO 3:
// Implement Gouraud shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 ambient;
out vec3 diffuse;
out vec3 specular;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialGloss;

uniform vec3 viewPos;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vec3 normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    
    vec3 lightDir = normalize(lightPos - worldPos.xyz);
    vec3 viewDir = normalize(viewPos - worldPos.xyz);

    ambient = lightAmbient * materialAmbient;
    diffuse = lightDiffuse * materialDiffuse * max(dot(lightDir, normal), 0.0);
    specular = lightSpecular * materialSpecular * pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), materialGloss);

    // tot_color = ambient + diffuse + specular;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}