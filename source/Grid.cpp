#include "Grid.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

void oglv::Grid::draw(Camera *camera) {
    m_shader.use();
    m_shader.set_uniform_mat4("view", camera->get_view());
    m_shader.set_uniform_mat4("projection", camera->get_projection());

    glUseProgram(m_shader.get_program_id());
    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size()));
    glBindVertexArray(0);
}
oglv::Grid::Grid() : m_shader("../shaders/grid.vert", "../shaders/grid.frag"), m_vao(0), m_vbo(0) {
    int i = 0;
    int z_vert[] = {-m_grid_ticks / 2, m_grid_ticks / 2};
    m_vertices.resize((m_grid_ticks + 1) * 2 * 3 * 2);
    // along x lines
    for (int x = -m_grid_ticks / 2; x <= m_grid_ticks / 2; x++) {
        for (int z : z_vert) {
            m_vertices[i] = static_cast<float>(x);     // x
            m_vertices[i + 1] = 0;                     // y
            m_vertices[i + 2] = static_cast<float>(z); // z
            i += 3;
        }
    }

    // along z lines
    int x_vert[] = {-m_grid_ticks / 2, m_grid_ticks / 2};
    for (int z = -m_grid_ticks / 2; z <= m_grid_ticks / 2; z++) {
        for (int x : x_vert) {
            m_vertices[i] = static_cast<float>(x);     // x
            m_vertices[i + 1] = 0;                     // y
            m_vertices[i + 2] = static_cast<float>(z); // z
            i += 3;
        }
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

oglv::Grid::~Grid() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}
