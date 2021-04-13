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
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


//--------------------------------------MAIN------------------------------------
int main(void)
{
    GLFWwindow* window;
    //Initialize the library
    if (!glfwInit())
        return -1;
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
        texture.Bind();
        //Renderer
        Renderer renderer;


        //--------------------------IMGUI----------------------------------
        ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();


        //Variables
        float r = 0.0f, increment = 0.05f;
        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        //---------------------------Rendering Loop---------------------------
        //Main loop the draws on screen.
        while (!glfwWindowShouldClose(window)) {
            
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            myShader.Bind();
            myShader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            myShader.SetUniform1i("u_Texture", 0);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model; 
            myShader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(vao, ib, myShader);

            model = glm::translate(glm::mat4(1.0f), translationB);
            mvp = proj * view * model;
            myShader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(vao, ib, myShader);

            if (r > 1.0f)
                increment = -0.05;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;
            

            //imgui frame
            {
                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }


            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}