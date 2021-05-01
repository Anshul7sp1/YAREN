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
	TestTexture2D::TestTexture2D() 
		: translation(200, 200, 0)
	{
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
		ib = std::make_unique<IndexBuffer>(indices, 6);
		vao = std::make_unique<VertexArray>();
		//Vertex array object creation
		vb = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexLayout layout;
		layout.Push(GL_FLOAT, 2);
		layout.Push(GL_FLOAT, 2);
		vao->AddBuffer(*vb, layout);
		//Creating IndexBuffer

		//Create a shader program that will work on the GPU and make
		//use of the provided vertex and index buffers.
		myShader = Shader("res/shaders/Basic.shader");
		myShader.CreateShaderProgram();
		//Textures
		texture  = std::make_unique<Texture>("res/textures/AMLogo.png");
		texture->Bind();
		myShader.Bind();
		myShader.SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D() {
		CallWithLog(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		CallWithLog(glClear(GL_COLOR_BUFFER_BIT));
	}

	//To be called in the rendering loop in main.
	void TestTexture2D::OnRender() {
		//set MVP matrix
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mvp = proj * view * model;
		myShader.Bind();
		myShader.SetUniformMat4f("u_MVP", mvp);
		Renderer r;
		r.Draw(*vao, *ib, myShader);
	}

	//To be called before calling the ImGui render in main loop.
	void TestTexture2D::OnImGuiRender() {
		ImGui::SliderFloat3("Translation A", &translation.x, 0.0f, 960.0f);
	}

	void TestTexture2D::OnUpdate(float deltaTime) {}

}