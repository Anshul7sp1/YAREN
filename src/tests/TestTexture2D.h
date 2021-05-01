#pragma once

#include "Test.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"


namespace test {
	class TestTexture2D : public Test {
	private:
		Shader myShader;
		std::unique_ptr<VertexArray> vao;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<Texture> texture;
		glm::vec3 translation;
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
	};
}