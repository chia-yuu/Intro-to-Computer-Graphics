#version 330 core

// TODO 3:
// Implement Gouraud shading

out vec4 FragColor;

in vec2 TexCoord;
in vec3 ambient;
in vec3 diffuse;
in vec3 specular;

uniform sampler2D ourTexture;

void main()
{
    vec4 texcolor = texture(ourTexture, TexCoord);
    vec3 tot_color = ambient * texcolor.rgb + diffuse * texcolor.rgb + specular;
    FragColor = vec4(tot_color, texcolor.a);
}