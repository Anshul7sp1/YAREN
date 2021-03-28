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