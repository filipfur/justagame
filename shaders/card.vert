#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform CameraUBO
{
    mat4 u_projection;
    mat4 u_view;
    vec3 u_eye_pos;
};

uniform mat4 u_model;
uniform float u_time;

uniform mat3 u_card_data;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;
out vec3 eyePos;

float BezierBlend(float t)
{
    return t * t * (3.0f - 2.0f * t);
}

void main()
{
    vec2 texOffset = u_card_data[0].xy;
    texCoord = aTexCoords + texOffset * step(0.2, dot(vec3(0,0,1), aNormal));

    mat3 normalMatrix = transpose(inverse(mat3(u_model)));
    normal = normalMatrix * aNormal;

    mat4 modelView = u_view * u_model;

    eyePos = u_eye_pos;

    //float t = cos(u_time + u_card_data.z) * 0.5 + 0.5;

    //t = BezierBlend(t);

    vec3 strive = u_card_data[1].xyz;

    float t = u_card_data[2].x;
    float r = u_card_data[2].y;

    // Billboard FTW!
    modelView[0][0] = mix(modelView[0][0], cos(r), t);
    modelView[0][1] = mix(modelView[0][1], -sin(r), t);
    modelView[0][2] = mix(modelView[0][2], 0.0, t);

    modelView[1][0] = mix(modelView[1][0], sin(r), t);
    modelView[1][1] = mix(modelView[1][1], cos(r), t);
    modelView[1][2] = mix(modelView[1][2], 0.0, t);

    modelView[2][0] = mix(modelView[2][0], 0.0, t);
    modelView[2][1] = mix(modelView[2][1], 0.0, t);
    modelView[2][2] = mix(modelView[2][2], 1.0, t);

    modelView[3][0] = mix(modelView[3][0], strive.x, t);
    modelView[3][1] = mix(modelView[3][1], strive.y, t);
    modelView[3][2] = mix(modelView[3][2], -2.4 + strive.x * 0.1 + strive.z, t);

    /*mat4 rotationMatrix = mat4(
        cos(radians(90.0)), -sin(radians(90.0)), 0.0, 0.0,
        sin(radians(90.0)), cos(radians(90.0)), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );*/

    fragPos = vec3(u_model * vec4(aPos, 1.0));
    gl_Position = u_projection * modelView * vec4(aPos, 1.0);
}