#version 330 core

#include <lightning.glsl>
#include lights.glsl

uniform vec4 u_color;
uniform float u_time;
uniform vec3 u_view_pos;
uniform vec4 u_base_color;
uniform float u_roughness;
uniform float u_metallic;

uniform samplerCube u_irradiance_map;

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

const float ao = 1.0;

void main()
{
    vec3 albedo = vec3(u_color * u_base_color);
    float metallic  = u_metallic;
    float roughness = u_roughness;

    vec3 N = normalize(normal);
    vec3 V = normalize(u_view_pos - fragPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < lights.length(); ++i) 
    {
        vec3 L = normalize(lights[i].position - fragPos);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].color * attenuation;

        float NDF = DistributionGGX(N, H, roughness);       
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
        vec3 specular = numerator / denominator;
  
        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    //vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
    vec3 kD = 1.0 - kS;
    vec3 irradiance = texture(u_irradiance_map, N).rgb;
    vec3 diffuse    = irradiance * albedo;
    vec3 ambient    = (kD * diffuse) * ao;

    vec3 color   = ambient + Lo;

    color = color / (color + vec3(1.0));
    fragColor = vec4(pow(color, vec3(1.0/2.2)), 1.0);
}