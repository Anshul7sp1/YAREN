#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0) {}

Shader::~Shader() {
    CallWithLog(glDeleteProgram(m_RendererID));
}

void Shader::setShaderPath(const std::string& file) {
    m_FilePath = file;
}

void Shader::Bind() const {
    if (!m_RendererID) std::cout << "[Warning]: Using default shader.\n";
    CallWithLog(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    CallWithLog(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v0) {
    CallWithLog(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string& name, float v0) {
    CallWithLog(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    CallWithLog(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    CallWithLog(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

//Finds and returns id of a uniform in Shader source along with warnings in case not found.
int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationMap.find(name) != m_UniformLocationMap.end())
        return m_UniformLocationMap[name];
    CallWithLog(int uniform_id = glGetUniformLocation(m_RendererID, name.c_str()));
    if (uniform_id == -1)
        std::cout << "[Warning]: Uniform \"" << name << "\" not found in shader \"" << m_FilePath << "\"\n";
    m_UniformLocationMap[name] = uniform_id;
    return uniform_id;
}

//Parses shader code from a .shader file into a shader source object. 
ShaderProgramSource Shader::ParseShader() {
    std::ifstream fin(m_FilePath);
    if (!fin)
        std::cout << "[Error]: Could not open .shader file.\n";
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType cur_type = ShaderType::NONE;

    size_t notFound = std::string::npos;
    while (getline(fin, line)) {
        if (line.find("#shader") != notFound) {
            if (line.find("vertex") != notFound)
                cur_type = ShaderType::VERTEX;
            else if (line.find("fragment") != notFound)
                cur_type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)cur_type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}


//Compiles a shader source into a glShader (with error feedback).
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    CallWithLog(glShaderSource(id, 1, &src, nullptr));
    CallWithLog(glCompileShader(id));

    int result;
    CallWithLog(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        CallWithLog(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char));
        CallWithLog(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << type << " shader\n" << "Info: " << message << std::endl;
        CallWithLog(glDeleteShader(id));
        return 0;
    }

    return id;
}

//Takes vertex and fragment shader source, compiles them using CompileShader 
//and links them into a program.
void Shader::CreateShaderProgram() {
    ShaderProgramSource src = this->ParseShader();
    std::string& vertexShader = src.VertexShader;
    std::string& fragmentShader = src.FragmentShader;

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    CallWithLog(glAttachShader(program, vs));
    CallWithLog(glAttachShader(program, fs));
    CallWithLog(glLinkProgram(program));
    CallWithLog(glValidateProgram(program));

    CallWithLog(glDeleteShader(vs));
    CallWithLog(glDeleteShader(fs));

    m_RendererID = program;
}