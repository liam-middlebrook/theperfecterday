#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow *window;

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

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

        // Start button for quit game
        if (buttons[7])
        {
            break;
        }

        // Update Input & Graphics
        glfwPollEvents();
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    return 0;
}
