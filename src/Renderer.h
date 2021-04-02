#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
//------------------------------------MACROS-----------------------------------

#define ASSERT(x) if(!(x)) __debugbreak();
#define CallWithLog(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//---------------------------------ERROR LOGGING--------------------------------

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

//Renderer Class
class Renderer {
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};