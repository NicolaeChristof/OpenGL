#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // Creates a shader object (enum shaderType)
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // Replaces the source code in a shader object (unsigned int shader, int count, char** string, int* length)
    glShaderSource(id, 1, &src, nullptr);
    // Compiles a shader object (unsigned int shaderID)
    glCompileShader(id);

    // Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        // Returns a parameter from a shader object (unsigned int shaderID, enum paramName, int* outparam)
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        // Returns the information log for a shader object (unsigned int shaderID, int length, int* length, char* message)
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Creates a program object
    unsigned int program = glCreateProgram();

    // Compile our shaders
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attaches a shader object to a program object (unsigned int programID, unsigned int shaderID)
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // Links a program object (unsigned int programID)
    glLinkProgram(program);
    // Validates a program object (unsigned int programID)
    glValidateProgram(program);

    // Deletes a shader object (unsigned int shaderID)
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the glfw library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // glewInit must be called *after* a valid context is created (above)
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error: glewInit did not return GLEW_OK" << std::endl;
        return -1;
    }

    // Print our driver version
    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;

    float positions[6] =
    {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // Declare an unsigned int to be our vertex buffer
    unsigned int buffer;
    // Generate buffer object names (int size, unsigned int* buffer)
    glGenBuffers(1, &buffer);
    // Bind means to select a buffer as our active vertex buffer
    // Bind a named buffer object (enum target, unsigned int buffer)
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Creates and initializes a buffer objects data store (enum target, size (bytes), optional pointer to data, enum usage)
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

    // Specify the layout of the buffer (vertex attributes)
    // (int index, int size, enum type, bool normalized, size(bytes) stride, (offset for first component) pointer)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    // Enable the vertex attribute array
    glEnableVertexAttribArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the currently bound buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}