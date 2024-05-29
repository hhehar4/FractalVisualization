#include "Shader.h"

unsigned int Shader::compileShader(unsigned int shaderType, const std::string& shaderPath)
{
    std::string shaderSource = readShader(shaderPath);

    unsigned int id = glCreateShader(shaderType);
    const char* src = shaderSource.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length + 1);
        glGetShaderInfoLog(id, length, &length, &message[0]);

        std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
        std::cout << &message[0] << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

std::string Shader::readShader(const std::string& filepath)
{
    std::string shaderString;
    std::ifstream stream(filepath);

    if (!stream)
    {
        std::cout << "Failed to open shader file: " << filepath << "\n";
        return "";
    }

    std::string line = "";
    while (getline(stream, line))
    {
        shaderString.append(line + '\n');
    }

    stream.close();
    return shaderString;
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    programID = glCreateProgram();
    unsigned int vertexShaderId = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
    unsigned int fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    glAttachShader(programID, vertexShaderId);
    glAttachShader(programID, fragmentShaderId);
    glLinkProgram(programID);
    glValidateProgram(programID);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

Shader::~Shader()
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
        programID = 0;
    }
}

void Shader::useProgram()
{
    glUseProgram(programID);
}
