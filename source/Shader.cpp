#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "glad/glad.h" // should be included before glfw3 because glfw needs glad
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "spdlog/spdlog.h"
#include "stb_image.h"

std::string oglv::Shader::_read_source(const std::string &path) {

    std::string code;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shader_file.open(path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        code = shader_stream.str();
    } catch (const std::ifstream::failure &e) {
        spdlog::error("shader file not sucessfully read");
    }
    return code;
}

oglv::Shader::Shader(const std::string &vertex_path, const std::string &frag_path) {
    const std::string vert_source = _read_source(vertex_path);
    const std::string frag_source = _read_source(frag_path);

    const char *shader_code_vert = vert_source.c_str();
    const char *shader_code_frag = frag_source.c_str();

    m_id_vert = glCreateShader(GL_VERTEX_SHADER);
    m_id_frag = glCreateShader(GL_FRAGMENT_SHADER);

    if (m_id_vert == 0 || m_id_frag == 0) {
        spdlog::error("failed to create shader");
        exit(-1);
    }
    m_id_program = glCreateProgram();

    // build vert
    glShaderSource(m_id_vert, 1, &shader_code_vert, nullptr);
    glCompileShader(m_id_vert);
    int success;
    glGetShaderiv(m_id_vert, GL_COMPILE_STATUS, &success);
    int log_size = 512;
    if (!success) {
        char log[log_size];
        glGetShaderInfoLog(m_id_vert, log_size, nullptr, log);
        spdlog::error("Compilation vertex shader error: ", std::string(log));
        exit(-1);
    }
    glAttachShader(m_id_program, m_id_vert);

    // build frag
    glShaderSource(m_id_frag, 1, &shader_code_frag, nullptr);
    glCompileShader(m_id_frag);
    glGetShaderiv(m_id_frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[log_size];
        glGetShaderInfoLog(m_id_frag, log_size, nullptr, log);
        spdlog::error("Compilation frag shader error {}", std::string(log));
        exit(-1);
    }
    glAttachShader(m_id_program, m_id_frag);

    // program
    glLinkProgram(m_id_program);
    glGetProgramiv(m_id_program, GL_LINK_STATUS, &success);

    if (!success) {
        char log[log_size];
        glGetProgramInfoLog(m_id_program, log_size, nullptr, log);
        spdlog::error("linking: {}", std::string(log));
    }
    glDeleteShader(m_id_vert);
    glDeleteShader(m_id_frag);
}

void oglv::Shader::generate_texture(const std::string &texture_path) {
    // load texture
    int width = 512, height = 512, channels = 3;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *image_data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);
    if (image_data == nullptr) {
        spdlog::error("unable to read the texture {}", texture_path);
        exit(-1);
    }

    // create GL texture
    glGenTextures(1, &m_id_texture);
    glBindTexture(GL_TEXTURE_2D, m_id_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image_data);
}

void oglv::Shader::set_uniform_bool(const std::string &name, bool value) const {
    GLint id = glGetUniformLocation(m_id_program, name.c_str());
    if (id == -1) {
        spdlog::error("error setting uniform bool");
        exit(-1);
    }
    glUniform1i(id, value);
}

void oglv::Shader::set_uniform_3f(const std::string &name, const glm::vec3 &value) const {
    GLint id = glGetUniformLocation(m_id_program, name.c_str());
    if (id == -1) {
        spdlog::error("error setting uniform 3f");
        exit(-1);
    }
    glUniform3f(id, value.x, value.y, value.z);
}

void oglv::Shader::set_uniform_1f(const std::string &name, float value) const {
    GLint id = glGetUniformLocation(m_id_program, name.c_str());
    if (id == -1) {
        spdlog::error("error setting uniform 1f");
        exit(-1);
    }
    glUniform1f(id, value);
}

void oglv::Shader::set_uniform_mat4(const std::string &name, const glm::mat4 &mat) const {

    GLint id = glGetUniformLocation(m_id_program, name.c_str());
    if (id == -1) {
        spdlog::error("error setting uniform mat4");
        exit(-1);
    }
    glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(mat));
}
