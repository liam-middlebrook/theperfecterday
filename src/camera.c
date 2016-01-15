#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

#include <stdio.h>
extern GLFWwindow* window;
glm::mat4 viewMat;
glm::mat4 projMat;

glm::mat4 getViewMat() {
    return viewMat;
}
glm::mat4 getProjMat() {
    return projMat;
}

glm::vec3 position(0.0f, 1.0f, 0.0f);

glm::vec3 getPosition() {
    return position;
}
void setPosition(glm::vec3 _position) {
    position = _position;
}

float horizAngle = 0.0f;
float vertAngle = 0.0f;
float fov = 3.14f / 6.0f;

float speed = 3.0f;
float mouseSpeed = 0.003f;

void camera_update() {
    static double prevTime = glfwGetTime();

    double time = glfwGetTime();
    float deltaTime = float(time - prevTime);
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwSetCursorPos(window, win_width / 2, win_height / 2);

    horizAngle += mouseSpeed * float(win_width / 2 - xPos);
    vertAngle += mouseSpeed * float(win_height / 2 - yPos);

    glm::vec3 direction(
        cos(vertAngle) * sin(horizAngle),
        sin(vertAngle),
        cos(vertAngle) * cos(horizAngle)
    );

    glm::vec3 right(
        sin(horizAngle - 3.14f / 2.0f),
        0,
        cos(horizAngle - 3.14f / 2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            position += right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            position -= right * deltaTime * speed;
    }

    projMat = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.001f, 1000.0f);

    viewMat = glm::lookAt(position, position+direction, up);

    prevTime = time;
}
