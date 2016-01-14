#define GLM_FORCE_RADIANS

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <libnoise/noise.h>

#include <iostream>
#include <fstream>
#include <string>

#include "camera.h"

// The Perfecter Day: WAT?
// The Perfecter Day is the spiritual successor/port of The Perfect Day by
// ajman1101 (Andrew Mandule). It combines all of the best features of The
// Perfect Day into one action-lacking adventure! As this is the spiritual
// successor to The Perfect Day, you naturally play as a ghost. As a ghost you
// don't need to worry about interacting with different objects in the world,
// because that isn't how being a ghost fucking works. Due to the importance of
// lava in the original The Perfect Day this game would be incomplete without
// different opprotunities to submerge yourself in the warmth and love of a nice
// lava bath.
// As I imagine the original The Perfect Day was written in only two Unity C#
// class files, so The Perfecter Day naturally only has two files that it is
// split into. Also because this is the spiritual successor to The Perfect Day,
// I'm going to go to Chipolte to think about what I've done to myself and the
// world.
    noise::module::Perlin perlinNoise;
GLuint loadShader(const char* vertLoc, const char* fragLoc);
GLuint loadTexture(const char* fileLoc);
GLfloat *genTerrainData(int *count, int width, int height);
void initSound(std::string file, std::string name);

    GLFWwindow *window;
void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, void *data);
int main(int argc, char** argv)
{

    glm::mat4 projectionMatrix;

    GLuint mainShader;
    GLuint waterShader;

    GLuint waterVAO;
    GLuint waterVBO;

    GLuint waterTex;
    GLuint waterMap;

    //GLuint terrainVAO;
    GLuint terrainVBO;

    GLuint terrainTex;

    // Declare some functions

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "The Perfecter Day", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = true;

    glewInit();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugMessageCallback, NULL);

    mainShader = loadShader("vert.glsl", "frag.glsl");
    waterShader = loadShader("water.vert.glsl", "water.frag.glsl");
    glUseProgram(waterShader);

    glClearColor(100.0f/ 255.0f, 149.0f/255.0f, 237.0f/255.0f, 1.0f);

    waterMap = loadTexture("wave.jpg");
    waterTex = loadTexture("pebbles2.jpg");
    terrainTex = loadTexture("grass.jpg");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, waterTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, terrainTex);

    glGenVertexArrays(1, &waterVAO);
    glBindVertexArray(waterVAO);

    glGenBuffers(1, &waterVBO);
    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);

    GLfloat waterQuad[6*5] =
    {
        // x y z u v
        -1.0f, +0.0f, -1.0f, +0.0f, +0.0f,
        -1.0f, +0.0f, +1.0f, +0.0f, +1.0f,
        +1.0f, +0.0f, +1.0f, +1.0f, +1.0f,

        +1.0f, +0.0f, +1.0f, +1.0f, +1.0f,
        +1.0f, +0.0f, -1.0f, +1.0f, +0.0f,
        -1.0f, +0.0f, -1.0f, +0.0f, +0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, waterQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const GLvoid*)(sizeof(GLfloat) * 3));
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(1);

    //glGenVertexArrays(1, &terrainVAO);
    //glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);

    GLfloat *terrainData;
    int terrainDataCount;

    terrainData = genTerrainData(&terrainDataCount, 20, 30);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * terrainDataCount, terrainData, GL_STATIC_DRAW);

    free(terrainData);

    glUseProgram(waterShader);
    glm::mat4 scale = glm::mat4(100.0f ,0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 100.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(
        glGetUniformLocation(waterShader, "modelMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(scale));
    glUseProgram(mainShader);
    scale = glm::mat4(75.0f ,0.0f, 0.0f, 0.0f,
                                0.0f, 5.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 75.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(
        glGetUniformLocation(mainShader, "modelMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(scale));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, win_width / 2, win_height / 2);
    while (!glfwWindowShouldClose(window))
    {

        //int count;
        //const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

        //// Start button for quit game
        //if (buttons[7])
        //{
        //    break;
        //}
        //const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

        //float x_axis = axes[6];
        //float y_axis = axes[7];

        //glm::vec3 vel = glm::vec3(x_axis, y_axis, 0.0f);
	    //glm::vec3 direction = glm::vec3(camera.rotMat * glm::vec4(vel, 1.0f));
        //camera.position += direction;
        //camera.rotation.x += 0.0001f;
        //
        glm::vec3 position = getPosition();
        position.y = 5 * perlinNoise.GetValue(position.x / 75.0f, 0.0f, position.z / 75.0f) + 1.0f;
        setPosition(position);
        camera_update();
        projectionMatrix = getProjMat();
        glm::mat4 viewMatrix = getViewMat();

        glUseProgram(waterShader);

    glUniformMatrix4fv(
        glGetUniformLocation(waterShader, "projectionMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(
            glGetUniformLocation(waterShader, "viewMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(viewMatrix));
        glUniform3f(
            glGetUniformLocation(waterShader, "texOffset"),
            0.03f * glfwGetTime(),
            0.04f * glfwGetTime(),
            0.0f);
        //printf("%f\n", glfwGetTime());

        glBindBuffer(GL_ARRAY_BUFFER, waterVBO);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const GLvoid*)(sizeof(GLfloat) * 3));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(1);

        glUseProgram(mainShader);
    glUniformMatrix4fv(
        glGetUniformLocation(mainShader, "projectionMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(
            glGetUniformLocation(mainShader, "viewMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(viewMatrix));
        glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const GLvoid*)(sizeof(GLfloat) * 3));
        glDrawArrays(GL_TRIANGLES, 0, 30*30*6*5);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(1);
//*/
        // Update Input & Graphics
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    const char* _source;
    const char* _type;
    const char* _severity;

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

GLuint loadTexture(const char* fileLoc)
{
	GLuint texture;

	FIBITMAP* bitmap = FreeImage_Load(
		FreeImage_GetFileType(fileLoc, 0),
		fileLoc);

	FIBITMAP* pImage;

	pImage = FreeImage_ConvertTo32Bits(bitmap);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FreeImage_GetWidth(pImage), FreeImage_GetHeight(pImage),
		0, GL_BGRA, GL_UNSIGNED_BYTE, static_cast<void*>(FreeImage_GetBits(pImage)));

	FreeImage_Unload(bitmap);
	FreeImage_Unload(pImage);

	return texture;
}

GLfloat *genTerrainData(int *count, int width, int height)
{
    *count = 5 * (width) * 6 * height;
    GLfloat *data = (GLfloat*)malloc(sizeof(GLfloat) * *count);

    //printf("Width: %d, Height: %d\n", width, height);

    if( data == NULL )
    {
        //printf("FUCK\n");
        return 0;
    }

    int idx = 0;
    for (float y = -1.0f; y < 1.0f - (2.0f / height); y += 2.0f / height)
    {
        //printf("row #: %f\n", y);
        for (float x = -1.0f; x < 1.0f; x += 2.0f / width)
        {
            // x y z u v
            //
            // 1 3
            // 2
            //  ^ v same tirangle
            //   2
            // 1 3
            data[idx + 0] = x;
            data[idx + 1] = perlinNoise.GetValue(x, 0.0f, y);
            data[idx + 2] = y;

            data[idx + 3] = (x + 1.0f) / 2.0f;
            data[idx + 4] = (y + 1.0f) / 2.0f;

            idx += 5;

            data[idx + 0] = x;
            data[idx + 1] = perlinNoise.GetValue(x, 0.0f, y + 2.0f / height);
            data[idx + 2] = y + 2.0f / height;

            data[idx + 3] = (x + 1.0f) / 2.0f;
            data[idx + 4] = (y + 1.0f + (2.0f / height)) / 2.0f;

            idx += 5;

            data[idx + 0] = x + 2.0f / width;
            data[idx + 1] = perlinNoise.GetValue(x + 2.0f / width, 0.0f, y);
            data[idx + 2] = y;

            data[idx + 3] = (x + 1.0f + (2.0f / width)) / 2.0f;
            data[idx + 4] = (y + 1.0f) / 2.0f;

            idx += 5;

            data[idx + 0] = x;
            data[idx + 1] = perlinNoise.GetValue(x, 0.0f, y + 2.0f / height);
            data[idx + 2] = y + 2.0f / height;

            data[idx + 3] = (x + 1.0f) / 2.0f;
            data[idx + 4] = (y + 1.0f + (2.0f / height)) / 2.0f;

            idx += 5;

            data[idx + 0] = x + 2.0f / width;
            data[idx + 1] = perlinNoise.GetValue(x + 2.0f / width, 0.0f, y);
            data[idx + 2] = y;

            data[idx + 3] = (x + 1.0f + (2.0f / width)) / 2.0f;
            data[idx + 4] = (y + 1.0f) / 2.0f;

            idx += 5;

            data[idx + 0] = x + 2.0f / width;
            data[idx + 1] = perlinNoise.GetValue(x + 2.0f / width, 0.0f, y + 2.0f / height);
            data[idx + 2] = y + 2.0f / height;

            data[idx + 3] = (x + 1.0f + (2.0f / width)) / 2.0f;
            data[idx + 4] = (y + 1.0f + (2.0f / height)) / 2.0f;

            idx += 5;

            //printf("%d out of %d\n", idx, *count);
        }
    }
    return data;
}
