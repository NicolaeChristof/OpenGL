// https://docs.gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the glfw library */
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "OpenGL Practice", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // turn on vsync
    glfwSwapInterval(1);

    // glewInit must be called *after* a valid context is created (above)
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error: glewInit did not return GLEW_OK" << std::endl;
        return -1;
    }

#if _DEBUG
    std::cout << "Driver version: " << glGetString(GL_VERSION) << std::endl;
#endif

    float positions[] =
    {
        -1.0f, -1.0f, 0.0f, 0.0f, // 0
         1.0f, -1.0f, 1.0f, 0.0f, // 1
         1.0f,  1.0f, 1.0f, 1.0f, // 2
        -1.0f,  1.0f, 0.0f, 1.0f  // 3
    };
#if _DEBUG
    std::cout << "sizeof positions: " << sizeof(positions) << std::endl;
#endif

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };
#if _DEBUG
    std::cout << "sizeof indicies: " << sizeof(indices) << std::endl;
    std::cout << "count of indicies: " << std::size(indices) << std::endl;
#endif

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    VertexArray va;
    VertexBuffer vb(positions, sizeof(positions));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, std::size(indices));

    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    Shader shader("resources/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    Texture texture("resources/textures/MadeInAbyss.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    // Unbind everything
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    glm::vec3 modeltranslation(0.0f, 0.0f, 0.0f);
    glm::vec3 viewTranslation(0.0f, 0.0f, 0.0f);

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), modeltranslation);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), -viewTranslation);
        glm::mat4 mvp = projection * view * model;

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);

        if (r > 1.0f)
        {
            increment = -0.05f;
        }
        else if (r < 0.0f)
        {
            increment = 0.05f;
        }

        r += increment;

        {
            ImGui::Begin("OpenGl Practice"); // Create a window and append into it.

            ImGui::Text("This is some useful text.");

            ImGui::SliderFloat2("Model", &modeltranslation.x, -2.0f, 2.0f); // Edit 1 float using a slider from lower bounds to upper bounds
            ImGui::SliderFloat2("View", &viewTranslation.x, -2.0f, 2.0f); // Edit 1 float using a slider from lower bounds to upper bounds

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}