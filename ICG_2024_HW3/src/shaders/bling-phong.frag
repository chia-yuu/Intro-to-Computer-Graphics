#version 330 core

// TODO 2
// Implement Bling-Phong shading

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D ourTexture;

uniform vec3 lightPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialGloss;

uniform vec3 viewPos;
uniform mat4 model;

void main()
{
    vec4 texcolor = texture(ourTexture, TexCoord);
    // vec3 normal = normalize(Normal);
    // vec4 worldPos = model * vec4(FragPos, 1.0);
    // vec3 wPos = worldPos.xyz;

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfway = normalize(lightDir + viewDir);

    vec3 ambient = lightAmbient * materialAmbient * texcolor.rgb;
    vec3 diffuse = lightDiffuse * materialDiffuse * max(dot(lightDir, Normal), 0.0) * texcolor.rgb;
    vec3 specular = lightSpecular * materialSpecular * pow(max(dot(Normal, halfway), 0.0), materialGloss);

    vec3 tot_color = ambient + diffuse + specular;
    FragColor = vec4(tot_color, texcolor.a);
}