#version 460 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;
in vec3 N;

uniform vec3 light_position;
uniform vec3 light_intensity;
uniform bool overlay;

void main() {

    float dot_product = dot(N, light_position);
    vec3 ambient = vec3(0.05);
    vec3 albedo = vec3(0.1);
    vec3 light_color = vec3(1, 1, 1);

    vec3 diffuse = ambient + albedo * light_intensity * max(dot_product, 0.0f);
    FragColor = vec4(diffuse.x * 2, diffuse.y, diffuse.z, 1.0f) * int(overlay) + vec4(diffuse.xyz, 1.0f) * (1 - int(overlay));
    float gamma_correction = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma_correction));
}
