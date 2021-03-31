#pragma once

#include <vector>
#include <GL/glew.h>

struct VertexAttribute {
	unsigned int type;
	unsigned int count;
	unsigned int normalized;
	unsigned int size;
};

class VertexLayout {
private:
	std::vector<VertexAttribute> m_Attributes;
	unsigned int m_Stride;
public:
	VertexLayout()
		: m_Stride(0) {}

	void Push(unsigned int gltype, unsigned int count);
	inline const std::vector<VertexAttribute>& GetAttributes() const { return m_Attributes; }
	inline unsigned int GetStride() const { return m_Stride; }
};