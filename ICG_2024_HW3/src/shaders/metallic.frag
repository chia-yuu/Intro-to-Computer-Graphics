#version 330 core

// TODO 5:
// Implement Metallic shading

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
	float bias = 0.2;
    float alpha = 0.4;
    float lightIntensity = 1.0;

    vec4 texcolor = texture(ourTexture, TexCoord);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = normalize(lightDir - 2.0 * dot(lightDir, Normal) * Normal);
    vec3 reflectDir = normalize(-viewDir - 2.0 * dot(-viewDir, Normal) * Normal);

    // diffuse
    float Bd = max(dot(lightDir, Normal), 0.0) * lightIntensity;
    float B = Bd + bias;

    // specular & ambient
    vec3 ambient = lightAmbient * materialAmbient;
    vec3 specular = lightSpecular * materialSpecular * pow(max(dot(viewDir, reflectDir), 0.0), materialGloss);

    vec3 tot_color = ambient * texcolor.rgb + specular + B * texcolor.rgb;

    // vec3 C_reflect = ambient + specular;
    // vec3 C_final = alpha * B * materialDiffuse + (1.0 - alpha) * C_reflect;
    // vec3 C_final = alpha * B * texcolor.rgb + (1.0 - alpha) * C_reflect;
    // vec3 model_color = tot_color * texcolor.rgb;
    vec3 reflect_color = texture(cubeMap, reflectDir).rgb;
    vec3 C_final = alpha * B * tot_color + (1.0 - alpha) * reflect_color;

    FragColor = vec4(C_final, texcolor.a);
    // FragColor = vec4(texcolor.rgb * C_final, texcolor.a);
}
