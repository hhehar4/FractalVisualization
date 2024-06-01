#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

int screenWidth = 1440;
int screenHeight = 900;
float verticalOffset = 0.0f;
float horizontalOffset = 0.0f;
float zoomScale = 1;
float baseTranslationIncrement = 0.025f;
float currentTranslationIncrement = 0.025f;
float zoomIncrement = 1.05f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
    
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
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        zoomScale *= zoomIncrement;
        currentTranslationIncrement = baseTranslationIncrement / zoomScale;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        zoomScale *= 1 / zoomIncrement;
        currentTranslationIncrement = baseTranslationIncrement / zoomScale;
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

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //float vertices[] = 
    //{
    //    -1.0f, -1.0f, 0.0f, 0.0f,
    //     1.0f, -1.0f, 1.0f, 0.0f,
    //    -1.0f,  1.0f, 0.0f, 1.0f,
    //     1.0f,  1.0f, 1.0f, 1.0f
    //};
    //unsigned int indices[] =    
    //{ 
    //    0, 1, 2,
    //    1, 2, 3
    //};

    float vertices[] =
    {
        //    x      y      z   
            -1.0f, -1.0f, -0.0f,
             1.0f,  1.0f, -0.0f,
            -1.0f,  1.0f, -0.0f,
             1.0f, -1.0f, -0.0f
    };

    unsigned int indices[] =
    {
        //  2---,1
        //  | .' |
        //  0'---3
            0, 1, 2,
            0, 3, 1
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    Shader shaders("res/shaders/shader.vert", "res/shaders/shader.frag");
    shaders.useProgram();

    int vOffsetLoc = glGetUniformLocation(shaders.programID, "verticalOffset");
    int hOffsetLoc = glGetUniformLocation(shaders.programID, "horizontalOffset");
    int zoomLoc = glGetUniformLocation(shaders.programID, "zoomScale");
    int screenSizeLoc = glGetUniformLocation(shaders.programID, "screenSize");
    
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