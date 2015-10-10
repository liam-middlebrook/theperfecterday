#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

GLFWwindow *window;
GLuint mainShader;

GLuint waterVAO;
GLuint waterVBO;

// Declare some functions

GLuint loadShader(const char* vertLoc, const char* fragLoc);
void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, void *data);
int main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "The Perfecter Day", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = true;

    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugMessageCallback, NULL);

    mainShader = loadShader("vert.glsl", "frag.glsl");

    glUseProgram(mainShader);

    glClearColor(100.0f/ 255.0f, 149.0f/255.0f, 237.0f/255.0f, 1.0f);


    glGenVertexArrays(1, &waterVAO);
    glBindVertexArray(waterVAO);

    glGenBuffers(1, &waterVBO);
    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);

    GLfloat waterQuad[6*5] =
    {
        // x y z u v
        -1.0f, -1.0f, +0.0f, +0.0f, +0.0f,
        -1.0f, +1.0f, +0.0f, +0.0f, +1.0f,
        +1.0f, +1.0f, +0.0f, +1.0f, +1.0f,

        +1.0f, +1.0f, +0.0f, +1.0f, +1.0f,
        +1.0f, -1.0f, +0.0f, +1.0f, +0.0f,
        -1.0f, -1.0f, +0.0f, +0.0f, +0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6  * 5, waterQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const GLvoid*)(sizeof(GLfloat) * 3));
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(1);
    while (!glfwWindowShouldClose(window))
    {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

        // Start button for quit game
        if (buttons[7])
        {
            break;
        }

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(1);

        // Update Input & Graphics
        glfwPollEvents();
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    return 0;
}

GLuint loadShader(const char* vertLoc, const char* fragLoc)
{
    // Load shader and shit
    std::ifstream vertIn(vertLoc);
    std::string vertexStr((std::istreambuf_iterator<char>(vertIn)),
                          std::istreambuf_iterator<char>());

    const char* vertexChars = vertexStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexChars, NULL);
    glCompileShader(vertexShader);

    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    printf("Vertex Shader Status %d\n", status);

    std::ifstream fragIn(fragLoc);
    std::string fragStr((std::istreambuf_iterator<char>(fragIn)),
                        std::istreambuf_iterator<char>());
    const char* fragmentChars = fragStr.c_str();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentChars, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    printf("Fragment Shader Status %d\n", status);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);

    return shaderProg;
}

void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, void *data)
{
    char* _source;
    char* _type;
    char* _severity;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

        case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

        case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

        default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

        case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

        case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

        case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

        case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

        default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

        case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

        case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

        default:
        _severity = "UNKNOWN";
        break;
    }

    printf("%d: %s of %s severity, raised from %s: %s\n",
            id, _type, _severity, _source, msg);
}
