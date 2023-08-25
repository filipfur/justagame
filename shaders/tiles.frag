#version 330 core

uniform sampler2D u_texture_0;
uniform vec4 u_color;
uniform int u_hovered_id;

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in float instanceId;

void main()
{
    vec3 color = vec3(0.6);
    if((instanceId + 1) == u_hovered_id)
    {
        float len = length(texCoord.xy - 0.5);
        color = mix(color, vec3(0.2, 0.9, 0.0), max(0.0, len));
    }
    vec4 c = vec4(color, 1.0) * u_color;
    fragColor = vec4(pow(c.rgb, vec3(1.0/2.2)), c.a);
    //fragColor = vec4(texCoord, 0.0, 1.0);
}