// https://docs.gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x)   GLClearError();\
                    x;\
                    ASSERT(GLCheckError(__FILE__, #x, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckError(const char* file, const char* function, int line)
{
    static bool success = true;
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << file << " " << function << " : " << line << std::endl;
        success = false;
    }
    return success;
}

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    // Opens file at filepath
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    // Read one line at a time
    std::string line;
    // Create an array to store our shader code
    std::stringstream ss[2]; // 0 for vertex, 1 for fragment
    // Track which "mode" we are in
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        // Find where each shader begins using our custom #shader tag
        if (line.find("#shader") != std::string::npos)
        {
            // Check which shader was found and assign type
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

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

#if _DEBUG
    // Print our driver version
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
    std::cout << "size of positions: " << sizeof(positions) << std::endl;
#endif

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };
#if _DEBUG
    std::cout << "size of indicies: " << sizeof(indices) << std::endl;
#endif

    // Declare an unsigned int to be our vertex buffer
    unsigned int vertexBuffer;
    // Generate buffer object names (int size, unsigned int* buffer)
    glGenBuffers(1, &vertexBuffer);
    // Bind means to select a buffer as our active vertex buffer
    // Bind a named buffer object (enum target, unsigned int buffer)
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Creates and initializes a buffer objects data store (enum target, size (bytes), optional pointer to data, enum usage)
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // Specify the layout of the buffer (vertex attributes)
    // (int index, int size, enum type, bool normalized, size(bytes) stride, (offset for first component) pointer)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    // Enable the vertex attribute array
    glEnableVertexAttribArray(0);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer); // index buffer binds to GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Parse shader source code
    ShaderProgramSource source = ParseShader("resources/shaders/Basic.shader");
    // Our CreateShader function returns a program id
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    // Installs a program object as part of current rendering state (unsigned int programID)
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the currently bound buffer
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Deletes a program object
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}