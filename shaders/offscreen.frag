#version 330 core

uniform int u_id;

out vec4 fragColor;

const float f = 1.0 / 256.0;

void main()
{
    fragColor = vec4(float(u_id) * f, 0.0, 0.0, 1.0);
}