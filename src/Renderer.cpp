#include "Renderer.h"
#include<iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    GLenum error = glGetError();
    if (error) {
        std::cout << "[OpenGL Error](" << error << ")\n[Function]" << function << "\n[File]" << file << "\n[Line]" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const {
    CallWithLog(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    va.Bind();
    shader.Bind();
    ib.Bind();
    CallWithLog(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
