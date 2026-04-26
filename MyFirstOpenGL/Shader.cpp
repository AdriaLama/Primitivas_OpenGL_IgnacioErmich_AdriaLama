#include "Shader.h"
#include <iostream>
#include <fstream>
#include <vector>

//Funcion que devolvera una string con todo el archivo leido
std::string Load_File(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::string fileContent;
    std::string line;

    //Lanzamos error si el archivo no se ha podido abrir
    if (!file.is_open())
    {
        std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    //Leemos el contenido y lo volcamos a la variable auxiliar
    while (std::getline(file, line))
    {
        fileContent += line + "\n";
    }

    //Cerramos stream de datos y devolvemos contenido
    file.close();
    return fileContent;
}

GLuint LoadFragmentShader(const std::string& filePath)
{
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string sShaderCode = Load_File(filePath);
    const char* cShaderSource = sShaderCode.c_str();

    glShaderSource(fragmentShader, 1, &cShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLint success;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (success)
    {
        return fragmentShader;
    }
    else
    {
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetShaderInfoLog(fragmentShader, logLength, nullptr, errorLog.data());
        std::cerr << "Se ha producido un error al cargar el fragment shader: " << errorLog.data() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

GLuint LoadGeometryShader(const std::string& filePath)
{
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    std::string sShaderCode = Load_File(filePath);
    const char* cShaderSource = sShaderCode.c_str();

    glShaderSource(geometryShader, 1, &cShaderSource, nullptr);
    glCompileShader(geometryShader);

    GLint success;
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

    if (success)
    {
        return geometryShader;
    }
    else
    {
        GLint logLength;
        glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetShaderInfoLog(geometryShader, logLength, nullptr, errorLog.data());
        std::cerr << "Se ha producido un error al cargar el vertex shader: " << errorLog.data() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

GLuint LoadVertexShader(const std::string& filePath)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    std::string sShaderCode = Load_File(filePath);
    const char* cShaderSource = sShaderCode.c_str();

    glShaderSource(vertexShader, 1, &cShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (success)
    {
        return vertexShader;
    }
    else
    {
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());
        std::cerr << "Se ha producido un error al cargar el vertex shader: " << errorLog.data() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

//Funcion que dado un struct que contiene los shaders de un programa generara el programa entero de la GPU
GLuint CreateProgram(const ShaderProgram& shaders)
{
    GLuint program = glCreateProgram();

    if (shaders.vertexShader != 0) glAttachShader(program, shaders.vertexShader);
    if (shaders.geometryShader != 0) glAttachShader(program, shaders.geometryShader);
    if (shaders.fragmentShader != 0) glAttachShader(program, shaders.fragmentShader);

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success)
    {
        if (shaders.vertexShader != 0) glDetachShader(program, shaders.vertexShader);
        if (shaders.geometryShader != 0) glDetachShader(program, shaders.geometryShader);
        if (shaders.fragmentShader != 0) glDetachShader(program, shaders.fragmentShader);

        return program;
    }
    else
    {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());
        std::cerr << "Error al linkar el programa: " << errorLog.data() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}