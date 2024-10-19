#version 460 core
layout (location = 0 ) in vec3 vertex_pos;

uniform mat4 view;
uniform mat4 projection;

void main() {

    gl_Position = projection * view * vec4(vertex_pos.x, vertex_pos.y, vertex_pos.z, 1.0);

};
