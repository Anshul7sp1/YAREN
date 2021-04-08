#include "VertexLayout.h"

//Adds an attribute i.e. a type and number of those required, to the vertex.
void VertexLayout::Push(unsigned int gltype, unsigned int count){
	switch (gltype) {
	case GL_FLOAT: 
		m_Attributes.push_back({ GL_FLOAT, count, GL_FALSE, sizeof(GLfloat) * count});
		m_Stride += count * sizeof(GLfloat);
		break;
	case GL_UNSIGNED_INT:
		m_Attributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, sizeof(GLuint) * count });
		m_Stride += count * sizeof(GLuint);
		break;
	case GL_UNSIGNED_BYTE:
		m_Attributes.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, sizeof(GLbyte) * count });
		m_Stride += count * sizeof(GLbyte);
		break;
	}
}

