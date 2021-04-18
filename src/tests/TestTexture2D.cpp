#include "TestTexture2D.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestTexture2D::TestTexture2D() {
	
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		//For transparent images
		CallWithLog(glEnable(GL_BLEND));
		CallWithLog(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//Vertex array object creation
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexLayout layout;
		layout.Push(GL_FLOAT, 2);
		layout.Push(GL_FLOAT, 2);
		VertexArray vao;
		vao.AddBuffer(vb, layout);
		vao.Unbind();
		//Creating IndexBuffer
		IndexBuffer ib(indices, 6);

		//Create a shader program that will work on the GPU and make
		//use of the provided vertex and index buffers.
		Shader myShader("res/shaders/Basic.shader");
		myShader.CreateShaderProgram();
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//Textures
		Texture texture("res/textures/AMLogo.png");
		//texture.Bind();
	}

	TestTexture2D::~TestTexture2D() {
		CallWithLog(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		CallWithLog(glClear(GL_COLOR_BUFFER_BIT));
	}

	//To be called in the rendering loop in main.
	void TestTexture2D::OnRender() {
		CallWithLog(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		CallWithLog(glClear(GL_COLOR_BUFFER_BIT));
	}

	//To be called before calling the ImGui render in main loop.
	void TestTexture2D::OnImGuiRender() {
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}

	void TestTexture2D::OnUpdate(float deltaTime) {}

}