#version 460 core

out vec4 FragColor;

uniform vec3 color;
uniform bool overlay;

void main() {
    vec3 c = color * (1 - int(overlay)) + color * 1.5 * int(overlay);
    FragColor = vec4(c.xyz, 1);
}
