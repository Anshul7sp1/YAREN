#include "TestClearColor.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {
	TestClearColor::TestClearColor() 
		:m_ClearColor {0.2f, 0.3f, 0.8f, 1.0f} {}

	void TestClearColor::OnRender() {
		CallWithLog(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		CallWithLog(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnImGuiRender() {
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}

	void TestClearColor::OnUpdate(float deltaTime) {}
	TestClearColor::~TestClearColor() {}

}