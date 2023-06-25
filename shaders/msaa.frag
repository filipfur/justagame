#version 330 core

out vec4 fragColor;

in vec2 texCoord;
uniform sampler2DMS u_texture;
uniform ivec2 u_resolution;

void main()
{
    vec4 diffuse = vec4(0.0);
    for(int i=0; i < 4; i++)
    {
        diffuse += texelFetch(u_texture, ivec2(texCoord * u_resolution), i);
    }
    fragColor = diffuse * 0.25f;
}