#version 330 core

uniform sampler2D u_texture_0;
uniform vec4 u_color;
uniform float u_time;
uniform vec3 u_view_pos;

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec3 color;

void main()
{
    vec4 c = vec4(color, 1.0) * u_color;
    fragColor = vec4(pow(c.rgb, vec3(1.0/2.2)), c.a);
    //fragColor = vec4(texCoord, 0.0, 1.0);
}