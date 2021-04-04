#pragma once

#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationMap;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void CreateShaderProgram();
	//set uniforms.
	void SetUniform1i(const std::string& name, int v0);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name,const glm::mat4& matrix);
	void SetUniform1f(const std::string& name, float v0);
private:
	ShaderProgramSource ParseShader();
	static unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);
};