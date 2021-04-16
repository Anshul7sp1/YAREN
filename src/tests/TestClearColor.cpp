#include "TestClearColor.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {
	TestClearColor::TestClearColor() 
		:m_ClearColor {0.2f, 0.3f, 0.8f, 1.0f} {}

	TestClearColor::~TestClearColor() {
		CallWithLog(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		CallWithLog(glClear(GL_COLOR_BUFFER_BIT));
	}

	//To be called in the rendering loop in main.
	void TestClearColor::OnRender() {
		CallWithLog(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		CallWithLog(glClear(GL_COLOR_BUFFER_BIT));
	}

	//To be called before calling the ImGui render in main loop.
	void TestClearColor::OnImGuiRender() {
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}

	void TestClearColor::OnUpdate(float deltaTime) {}

}