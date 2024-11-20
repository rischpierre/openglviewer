#ifndef OPENGLVIEWER_SHADER_H
#define OPENGLVIEWER_SHADER_H

#include "glad/glad.h" // should be included before glfw3 because glfw needs glad
#include "glm/glm.hpp"
#include <string>

namespace oglv {
class Shader {
public:
    Shader(const std::string &vertex_path, const std::string &frag_path);

    /// Call to openGl to use this shader
    void use() const { glUseProgram(m_id_program); };

    /// load a texture from a path and pass it to openGL
    void generate_texture(const std::string &texture_path);

    [[nodiscard]] GLuint get_program_id() const { return m_id_program; };
    [[nodiscard]] GLuint get_id_texture() const { return m_id_texture; };

    /// functions to set uniforms on the shaders
    void set_uniform_bool(const std::string &name, bool value) const;
    void set_uniform_3f(const std::string &name, const glm::vec3 &value) const;
    void set_uniform_1f(const std::string &name, float value) const;
    void set_uniform_mat4(const std::string &name, const glm::mat4 &mat) const;

private:
    /// Returns the source code of the glsl shaders
    static std::string _read_source_code(const std::string &path);

    /// Various ids for the openGL shaders
    GLuint m_id_vert = 0;
    GLuint m_id_frag = 0;
    GLuint m_id_program = 0;
    GLuint m_id_texture = 0;
};
} // namespace oglv

#endif // OPENGLVIEWER_SHADER_H
