#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>


//A shader source object that contains vertex and fragment shader source code.
struct ShaderProgramSource {
    std::string VertexShader;
    std::string FragmentShader;
};

//Parses shader code from a .shader file into a shader source object. 
static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream fin(filepath);
    if (!fin)
        std::cout << "Error: Could not open .shader file. Using default shader instead\n";
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
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << type << " shader\n" << "Info: "<< message <<std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

//Takes vertex and fragment shader source, compiles them using CompileShader 
//and links them into a program.
static unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

//Overload for CreateShaderProgram. Takes a shader source file, parses it and calls previous overload.
static unsigned int CreateShaderProgram(const std::string& file) {
    ShaderProgramSource src = ParseShader(file);
    return CreateShaderProgram(src.VertexShader, src.FragmentShader);
}


//--------------------------------------MAIN------------------------------------
int main(void)
{
    GLFWwindow* window;

    //Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    //vertex and index buffers.
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };
    int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    //Initialize, bind and populate the vertex glBuffer.
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
    //Explain the type and size of different attriibutes that are in a vertex using id.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    //Initialize, bind and populate the index glBuffer.
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    //Create a shader program that will work on the GPU and make
    //use of the provided vertex and index buffers.
    unsigned int shaderProgram = CreateShaderProgram("res/shaders/Basic.shader");
    glUseProgram(shaderProgram);

    //Main loop the draws on screen.
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);  
        glfwPollEvents();
    }

    //Freeing resoures.
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}