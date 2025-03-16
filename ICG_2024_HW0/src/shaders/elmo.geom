#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out; // 根據毛髮數量調整 max_vertices = 3（三角形原始頂點數） + 3 * hairCount（每頂點毛髮）

out vec4 Color; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float hairLength; // 控制毛髮的長度
uniform vec3 objectColor;
uniform int hairCount; // 新增：控制每頂點毛髮數量

in VS_OUT {
    vec4 Normal;
    vec2 texCoord;
    vec4 worldPos;
} gs_in[];

void main() {
    // Cube
    for (int i = 0; i < 3; i++) {
        Color = vec4(objectColor, 1.0);
        gl_Position = projection * view * gs_in[i].worldPos;
        EmitVertex();
    }

    // Add hair
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < hairCount; j++) {
            // hair root
            Color = vec4(objectColor, 1.0);
            gl_Position = projection * view * gs_in[i].worldPos;
            EmitVertex();

            // hair direction with variation
            vec3 baseNormal = normalize(vec3(gs_in[i].Normal));
            float angle = radians(360.0 * j / float(hairCount)); // 均勻分佈角度
            vec3 offset = vec3(cos(angle), sin(angle), 0.0); // 偏移方向
            vec3 hairDirection = normalize(baseNormal + 0.1 * offset); // 偏移增加隨機性

            // hair tip
            vec4 hairTip = gs_in[i].worldPos + vec4(hairDirection, 0.0) * hairLength;
            Color = vec4(objectColor, 1.0); // (white color or object color) 再看看
            gl_Position = projection * view * hairTip;
            EmitVertex();
        }
    }
    EndPrimitive();
}
