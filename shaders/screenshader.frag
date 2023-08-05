#version 330 core

out vec4 fragColor;

in vec2 texCoord;
uniform sampler2D u_texture;

void main()
{
    vec3 color = texture(u_texture, texCoord).rgb;
    fragColor = vec4(vec3(pow(texCoord.y, 1.0 / 4.0) * 0.8), 1.0);
    fragColor = vec4(color, 1.0);
}