#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform bool has_txt;       // use texture or color

void main()
{
    if(!has_txt){
        FragColor = vec4(objectColor, 1.0);
    }
    else{
        FragColor = texture(ourTexture, TexCoord);
    }
} 