#ifndef OPENGLVIEWER_GRID_H
#define OPENGLVIEWER_GRID_H

#include "Camera.hpp"
#include "Shader.hpp"
#include <vector>

namespace oglv {
class Grid {
public:
    Grid();
    ~Grid();
    void draw(Camera *camera);

private:
    int m_grid_ticks = 20;
    std::vector<float> m_vertices;
    unsigned int m_vao, m_vbo;
    Shader m_shader;
};
} // namespace oglv

#endif // OPENGLVIEWER_GRID_H
