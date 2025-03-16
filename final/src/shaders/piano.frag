#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 worldPos;

out vec4 FragColor;

uniform sampler2D heliTexture; 
uniform samplerCube skybox;

uniform int white;
uniform vec3 objectColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float gloss;
};

uniform Light light;
uniform Material material;
uniform vec3 cameraPos;

const float bias = 0.2;
const float alpha = 0.4;

void main()
{
    vec3 L = normalize(light.position - FragPos);
    vec3 V = normalize(cameraPos - FragPos);
    vec3 N = normalize(Normal);
    vec3 R = reflect(-V, N);

    vec3 envColor = texture(skybox, R).rgb;
    float diff = max(dot(L, N), 0.0);
    vec3 diffuse = diff * light.diffuse;

    vec3 baseColor = texture(heliTexture, TexCoords).rgb;
    vec3 ambient = light.ambient * baseColor;
    vec3 colorWithBias = (diffuse + bias) * baseColor;

    vec3 finalColor;
    if (white == 0) {
        vec3 whiteBase = vec3(1.0);
        finalColor = mix(whiteBase, envColor, 0.5);
    } else if (white == 1) {
        vec3 blackBase = vec3(0.0);
        finalColor = mix(blackBase, envColor, 0.5);
    } else if(white ==3){
        ambient = vec3(0.2f, 0.2f, 0.2f) * objectColor;
        diffuse = light.diffuse * max(dot(N, L), 0.0) * objectColor;

        finalColor = ambient + diffuse;

        // finalColor = objectColor;
    }else {
        finalColor = alpha * (ambient + colorWithBias) + (1.0 - alpha) * envColor;
    }

    FragColor = vec4(finalColor, 1.0);
}