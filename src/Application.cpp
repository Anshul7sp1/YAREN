#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

//------------------------------------MACROS-----------------------------------

#define ASSERT(x) if(!(x)) __debugbreak();
#define CallWithLog(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//---------------------------------ERROR LOGGING--------------------------------

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line) {
    GLenum error = glGetError();
    if (error) {
        std::cout << "[OpenGL Error](" << error << ")\n[Function]" << function<< "\n[File]" << file << "\n[Line]" << line << std::endl;
        return false;
    }
    return true;
}

//---------------------------------SHADER PROGRAM-------------------------------

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
    CallWithLog(glShaderSource(id, 1, &src, nullptr));
    CallWithLog(glCompileShader(id));

    int result;
    CallWithLog(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        CallWithLog(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char));
        CallWithLog(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << type << " shader\n" << "Info: "<< message <<std::endl;
        CallWithLog(glDeleteShader(id));
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

    CallWithLog(glAttachShader(program, vs));
    CallWithLog(glAttachShader(program, fs));
    CallWithLog(glLinkProgram(program));
    CallWithLog(glValidateProgram(program));

    CallWithLog(glDeleteShader(vs));
    CallWithLog(glDeleteShader(fs));

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
    glfwSwapInterval(1);
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

    //Initialize vertex array object
    unsigned int vao;
    CallWithLog(glGenVertexArrays(1, &vao));
    CallWithLog(glBindVertexArray(vao));

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

    //using "uniforms" in the shader program
    CallWithLog(int uniform_id = glGetUniformLocation(shaderProgram, "u_Color"));
    ASSERT(uniform_id != -1);
    CallWithLog(glUniform4f(uniform_id, 0.2f, 0.3f, 0.8f, 1.0f));

    //Main loop the draws on screen.
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        CallWithLog(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        glfwSwapBuffers(window);  
        glfwPollEvents();
    }

    //Freeing resoures.
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}