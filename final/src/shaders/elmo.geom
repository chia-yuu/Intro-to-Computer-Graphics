#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 99) out; // 更新為考慮所有毛髮數量

out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float hairLength; // 控制毛髮的長度
uniform vec3 objectColor;
uniform int hairCount; // 每頂點毛髮數量

in VS_OUT {
    vec4 Normal;
    vec2 texCoord;
    vec4 worldPos;
} gs_in[];

void generateHair(vec4 basePoint, vec3 baseNormal) {
    for (int j = 0; j < hairCount; j++) {
        // 根位置
        Color = vec4(objectColor, 1.0);
        gl_Position = projection * view * basePoint;
        EmitVertex();

        // 毛髮方向及角度計算
        float angle = radians(360.0 * j / float(hairCount)); // 均勻分佈角度
        vec3 randomOffset = vec3(cos(angle), sin(angle), 0.1); // 添加輕微隨機性
        vec3 hairDirection = normalize(baseNormal + 0.2 * randomOffset);

        // 毛髮末端位置
        vec4 hairTip = basePoint + vec4(hairDirection, 0.0) * hairLength;
        Color = vec4(objectColor, 1.0); 
        gl_Position = projection * view * hairTip;
        EmitVertex();

        EndPrimitive();
    }
}

void main() {
    // 繪製三角形頂點
    for (int i = 0; i < 3; i++) {
        Color = vec4(objectColor, 1.0);
        gl_Position = projection * view * gs_in[i].worldPos;
        EmitVertex();
    }

    // 對三角形三個頂點生成毛髮
    for (int i = 0; i < 3; i++) {
        vec3 normal = normalize(vec3(gs_in[i].Normal));
        generateHair(gs_in[i].worldPos, normal);
    }

    // 計算三角形內部中點
    vec4 midPoint = (gs_in[0].worldPos + gs_in[1].worldPos + gs_in[2].worldPos) / 3.0;
    vec3 midNormal = normalize(
        vec3(gs_in[0].Normal) +
        vec3(gs_in[1].Normal) +
        vec3(gs_in[2].Normal)
    );

    // 在中點生成毛髮
    generateHair(midPoint, midNormal);

    // 計算每邊的中點
    vec4 edgeMidPoints[3];
    vec3 edgeNormals[3];
    for (int i = 0; i < 3; i++) {
        int next = (i + 1) % 3;
        edgeMidPoints[i] = (gs_in[i].worldPos + gs_in[next].worldPos) / 2.0;
        edgeNormals[i] = normalize(vec3(gs_in[i].Normal) + vec3(gs_in[next].Normal));
    }

    // 在三邊中點生成毛髮
    for (int i = 0; i < 3; i++) {
        generateHair(edgeMidPoints[i], edgeNormals[i]);
    }
}