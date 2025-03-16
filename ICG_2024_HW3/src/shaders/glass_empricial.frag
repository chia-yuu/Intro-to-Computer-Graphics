#version 330 core

// TODO 6-2
// Implement Glass-Empricial shading
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform samplerCube cubeMap;

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
    float AIR_coeff = 1.0;
    float GLASS_coeff = 1.52;
    float n = AIR_coeff / GLASS_coeff;
    float scale = 0.7;
    float power = 2.0;
    float bias = 0.2;
    vec4 texcolor = texture(ourTexture, TexCoord);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = normalize(-viewDir - 2.0 * dot(-viewDir, Normal) * Normal);

    float R_theta = max(0.0, min(1.0, bias + scale * pow((1 + dot(-viewDir, Normal)), power)));

    float K = 1 - n * n * (1 - pow(dot(-viewDir, Normal), 2));
    vec3 T;
    if(K < 0){
        T = vec3(0.0, 0.0, 0.0);
    }
    else{
        T = n * (-viewDir) - (n * dot(-viewDir, Normal) + pow(K, 1/2)) * Normal;
    }

    vec3 reflect_color = texture(cubeMap, reflectDir).rgb;
    vec3 refract_color = texture(cubeMap, T).rgb;

    vec3 C_final = R_theta * reflect_color + (1 - R_theta) * refract_color;
    // vec3 C_final = texcolor.rgb;
    FragColor = vec4(C_final, texcolor.a);
    // FragColor = texcolor;
}