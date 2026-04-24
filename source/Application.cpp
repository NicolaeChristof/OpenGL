// https://docs.gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "examples/ExampleClearColor.h"
#include "examples/ExampleTexture2D.h"

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

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    // ImGui Initialization
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    // Set up Example Menu gui
    example::Example* currentExample = nullptr;
    example::ExampleMenu* exampleMenu = new example::ExampleMenu(currentExample);
    currentExample = exampleMenu;

    // Register examples
    exampleMenu->RegisterExample<example::ExampleClearColor>("Clear Color");
    exampleMenu->RegisterExample<example::ExampleTexture2D>("Texture 2D");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render current example
        if (currentExample)
        {
            currentExample->OnUpdate(0.0f);
            currentExample->OnRender();
            ImGui::Begin("Example");
            if (currentExample != exampleMenu && ImGui::Button("<-"))
            {
                delete currentExample;
                currentExample = exampleMenu;
            }
            currentExample->OnImGuiRender();
            ImGui::End();
        }

        // ImGui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    delete currentExample;
    if (currentExample != exampleMenu) delete exampleMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}