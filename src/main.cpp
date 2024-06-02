#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

int screenWidth = 900;
int screenHeight = 900;
float verticalOffset = 0.0f;
float horizontalOffset = 0.0f;
float zoomScale = 1;
float baseTranslationIncrement = 0.05f;
float currentTranslationIncrement = 0.05f;
float zoomIncrement = 1.1f;
bool showJulia = false;
float mouseXConv = 0.0;
float mouseYConv = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    zoomScale = (yOffset > 0 ? zoomScale * zoomIncrement : zoomScale / zoomIncrement);
    currentTranslationIncrement = baseTranslationIncrement / zoomScale;
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
    double mouseXPos, mouseYPos;
    glfwGetCursorPos(window, &mouseXPos, &mouseYPos);

    float aspectRatio = float(screenWidth / screenHeight);

    double normalizedX = ((mouseXPos / screenWidth) - 0.5) * 2;   
    double normalizedY = ((mouseYPos / screenHeight) - 0.5) * 2;
    
    mouseXConv = float(2 * aspectRatio * (1 / zoomScale) * normalizedX + horizontalOffset);
    // Multiply by -1 and subtract offset since gl_FragCoord in fragment shader has the origin at the bottom left while glfwGetCursorPos has origin at top left
    mouseYConv = -1 * float(2 * (1 / zoomScale) * normalizedY - verticalOffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        verticalOffset += currentTranslationIncrement;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        verticalOffset -= currentTranslationIncrement;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        horizontalOffset -= currentTranslationIncrement;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        horizontalOffset += currentTranslationIncrement;
    }
    if ((key == GLFW_KEY_F) && action == GLFW_PRESS) {
        showJulia = !showJulia;
    }
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(screenWidth, screenHeight, "Fractal Visualization", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    float vertices[] =
    {
        -1.0f, -1.0f, -0.0f,
         1.0f,  1.0f, -0.0f,
        -1.0f,  1.0f, -0.0f,
         1.0f, -1.0f, -0.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        0, 3, 1
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    Shader shaders("res/shaders/shader.vert", "res/shaders/shader.frag");
    shaders.useProgram();

    int vOffsetLoc = glGetUniformLocation(shaders.programID, "verticalOffset");
    int hOffsetLoc = glGetUniformLocation(shaders.programID, "horizontalOffset");
    int zoomLoc = glGetUniformLocation(shaders.programID, "zoomScale");
    int screenSizeLoc = glGetUniformLocation(shaders.programID, "screenSize");
    int showJuliaLoc = glGetUniformLocation(shaders.programID, "showJulia");
    int mousePosLoc = glGetUniformLocation(shaders.programID, "mousePos");
    
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaders.useProgram();

        glUniform1f(vOffsetLoc, verticalOffset);
        glUniform1f(hOffsetLoc, horizontalOffset);
        glUniform1f(zoomLoc, 1 / zoomScale);
        glUniform2f(screenSizeLoc, float(screenWidth), float(screenHeight));
        glUniform1i(showJuliaLoc, showJulia);
        glUniform2f(mousePosLoc, mouseXConv, mouseYConv);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}