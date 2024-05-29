#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
private:
    unsigned int compileShader(unsigned int shaderType, const std::string& shaderSource);

    std::string readShader(const std::string& filepath);

public:
    unsigned int programID;
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    ~Shader();

    void useProgram();
};