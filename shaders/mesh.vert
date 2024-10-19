#version 460 core
layout (location = 0 ) in vec3 vertex_pos;
layout (location = 1 ) in vec3 vertex_normal;
layout (location = 2 ) in vec2 texcoord;

out vec4 vertexColor;
out vec2 texCoord;
out vec3 N;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

    gl_Position = projection * view * model * vec4(vertex_pos.x, vertex_pos.y, vertex_pos.z, 1.0);
    vertexColor = vec4(vertex_pos.x, vertex_pos.y, vertex_pos.z, 1.0);
    texCoord = texcoord;
    N = vertex_normal;

};
