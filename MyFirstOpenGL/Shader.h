#pragma once
#include <GL/glew.h>
#include <string>

struct ShaderProgram
{
    GLuint vertexShader = 0;
    GLuint geometryShader = 0;
    GLuint fragmentShader = 0;
};

std::string Load_File(const std::string& filePath);

GLuint LoadVertexShader(const std::string& filePath);
GLuint LoadGeometryShader(const std::string& filePath);
GLuint LoadFragmentShader(const std::string& filePath);

GLuint CreateProgram(const ShaderProgram& shaders);
