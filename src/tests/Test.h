#pragma once
#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace test {
	
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

	};

	class TestMenu : public Test {
	public: 
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;
		template <typename T>
		void RegisterTest(const std::string& name) {
			std::cout << "[Registering class]: " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T; }));
		}
	private:
		//pointer to the test currently running
		Test*& m_CurrentTest;

		//List of all possible tests with their names and ctors
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;

	};
}