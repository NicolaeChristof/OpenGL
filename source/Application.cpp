// https://docs.gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Shader.h"

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
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

    VertexArray va;
    VertexBuffer vb(positions, sizeof(positions));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, std::size(indices));

    Shader shader("resources/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    // Unbind everything
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind program
        shader.Bind();
        // Set uniforms
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        // Bind vertex array
        va.Bind();
        // Bind index buffer
        ib.Bind();

        // Draw the currently bound buffer
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        if (r > 1.0f)
        {
            increment = -0.05f;
        }
        else if (r < 0.0f)
        {
            increment = 0.05f;
        }

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}