#version 330 core

out vec4 fragColor;

in float instanceId;
const float f = 1.0 / 256.0;

void main()
{
    fragColor = vec4(float(instanceId + 1.0) * f, 0.0, 0.0, 1.0);
}