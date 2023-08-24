#version 330 core

uniform sampler2D u_texture_0;
uniform vec4 u_color;
uniform float u_time;
uniform vec3 u_view_pos;

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

void main()
{
    vec4 color = texture(u_texture_0, texCoord) * u_color;
    fragColor = vec4(pow(color.rgb, vec3(1.0/2.2)), color.a);
    fragColor.rgb = vec3(0.7);
}