#version 460 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;
in vec3 N;

uniform vec3 light_position;
uniform vec3 light_intensity;
uniform bool overlay;
uniform bool activate_texture;
uniform sampler2D sampler;

void main() {

    float gamma_correction = 2.2;

    float dot_product = dot(N, light_position);

    vec3 ambient = vec3(0.01);

    vec3 albedo = texture(sampler, texCoord).xyz * int(activate_texture) + vec3(0.3) * (1 - int(activate_texture));

    albedo = pow(albedo, vec3(gamma_correction)); // revese correction for srgb textures

    vec3 diffuse = ambient + albedo * light_intensity * max(dot_product, 0.0f);

    FragColor = vec4(diffuse.x * 2, diffuse.y, diffuse.z, 1.0f) * int(overlay) + vec4(diffuse.xyz, 1.0f) * (1 - int(overlay));
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma_correction));
}
