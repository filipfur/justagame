#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;
uniform float u_time;

uniform vec4 u_card_data;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

float BezierBlend(float t)
{
    return t * t * (3.0f - 2.0f * t);
}

void main()
{
    texCoord = aTexCoords + u_card_data.xy * step(0.2, dot(vec3(0,0,1), aNormal));

    mat3 normalMatrix = transpose(inverse(mat3(u_model)));
    normal = normalMatrix * aNormal;

    mat4 modelView = u_view * u_model;

    float t = cos(u_time + u_card_data.z) * 0.5 + 0.5;

    t = BezierBlend(t);

    // Billboard FTW!
    modelView[0][0] = mix(modelView[0][0], 1.0, t);
    modelView[0][1] = mix(modelView[0][1], 0.0, t);
    modelView[0][2] = mix(modelView[0][2], 0.0, t);

    modelView[1][0] = mix(modelView[1][0], 0.0, t);
    modelView[1][1] = mix(modelView[1][1], 1.0, t);
    modelView[1][2] = mix(modelView[1][2], 0.0, t);

    modelView[2][0] = mix(modelView[2][0], 0.0, t);
    modelView[2][1] = mix(modelView[2][1], 0.0, t);
    modelView[2][2] = mix(modelView[2][2], 1.0, t);

    modelView[3][0] = mix(modelView[3][0], u_card_data.z, t);
    modelView[3][1] = mix(modelView[3][1], u_card_data.w, t);
    modelView[3][2] = mix(modelView[3][2], -2.4 + u_card_data.z * 0.1, t);

    fragPos = vec3(u_model * vec4(aPos, 1.0));
    gl_Position = u_projection * modelView * vec4(aPos, 1.0);
}