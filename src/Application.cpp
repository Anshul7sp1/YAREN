#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


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
    
    {
        //vertex and index buffers.
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        //Vertex array object creation
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexLayout layout;
        layout.Push(GL_FLOAT, 2);
        VertexArray vao;
        vao.AddBuffer(vb, layout);
        //Creating IndexBuffer
        IndexBuffer ib(indices, 6);

        //Create a shader program that will work on the GPU and make
        //use of the provided vertex and index buffers.
        Shader myShader("res/shaders/Basic.shader");
        myShader.CreateShaderProgram();
        myShader.Bind();

        float r = 0.0f, increment = 0.05f;

        //Main loop the draws on screen.
        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);
            
            myShader.SetUniform4f("u_Color", r, 1-r, 0.8f, 1.0f);
            vao.Bind();
            ib.Bind();
            CallWithLog(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
            
            if (r > 1.0f)
                increment = -0.05;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}