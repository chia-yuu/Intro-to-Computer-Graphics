#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT{
    vec4 Normal;
    vec2 texCoord;
    vec4 worldPos;
} vs_out;  

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.texCoord = aTexCoord;
    vs_out.Normal = model * vec4(aNormal, 0.0);
    vs_out.worldPos = model * vec4(aPos, 1.0);
    gl_Position = projection * view *vs_out.worldPos;
}