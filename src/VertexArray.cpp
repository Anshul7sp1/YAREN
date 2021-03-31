#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray() {
	CallWithLog(glGenVertexArrays(1, &m_RendererID));
}
VertexArray::~VertexArray() {
	CallWithLog(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const {
	CallWithLog(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	CallWithLog(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexLayout& layout) {
	vb.Bind();
	const std::vector<VertexAttribute>& attributes = layout.GetAttributes();
	unsigned int i = 0, offset = 0;
	for (const auto& attribute : attributes) {
		CallWithLog(glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalized, 
			layout.GetStride(), (const void*)offset));
		CallWithLog(glEnableVertexAttribArray(i));
		i++;
		offset += attribute.size;
	}
	
}