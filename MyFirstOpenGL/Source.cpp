#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#define ID_CUBO 0
#define ID_ORTOEDRO 1
#define ID_PIRAMIDE 2

std::vector<GLuint> compiledPrograms;

//Struct para controlar cada figura de la escena
struct GameObject
{
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 rotation = glm::vec3(0.f);
    glm::vec3 scale = glm::vec3(1.f);
    glm::vec3 forward = glm::vec3(1.f, 0.f, 0.f);
    float fVelocity = 0.01f;
    float fAngularVel = 1.0f;
};

struct ShaderProgram
{
    GLuint vertexShader = 0;
    GLuint geometryShader = 0;
    GLuint fragmentShader = 0;
};

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight)
{
    //Definir nuevo tamaño del viewport
    glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
    glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
}

//Funcion que devolvera una string con todo el archivo leido
std::string Load_File(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::string   fileContent;
    std::string   line;

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

//Funcion que genera una matriz de traslacion representada por un vector
glm::mat4 GenerateTranslationMatrix(glm::vec3 translation)
{
    return glm::translate(glm::mat4(1.0f), translation);
}

//Funcion que genera una matriz de rotacion dado un angulo y un vector
glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees)
{
    return glm::rotate(glm::mat4(1.0f), glm::radians(fDegrees), glm::normalize(axis));
}

//Funcion que genera una matriz de escalado representada por un vector
glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis)
{
    return glm::scale(glm::mat4(1.0f), scaleAxis);
}

void main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Primitivas 3D", NULL, NULL);

    glfwSetFramebufferSizeCallback(window, Resize_Window);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (glewInit() == GLEW_OK)
    {
        //Declarar instancias de GameObject para cada figura
        GameObject cube;
        GameObject ortho;
        GameObject pyramid;

        //Posicionamos cada figura en su lugar de la ventana
        cube.position = glm::vec3(-0.55f, 0.f, 0.f);
        ortho.position = glm::vec3(0.0f, 0.f, 0.f);
        pyramid.position = glm::vec3(0.55f, 0.f, 0.f);

        cube.forward = glm::vec3(0.f, 1.f, 0.f);

        //Escala uniforme para cubo y piramide
        cube.scale = glm::vec3(0.3f, 0.3f, 0.3f);
        //Ortoedro: mas alto que ancho para diferenciarse visualmente del cubo
        ortho.scale = glm::vec3(0.15f, 0.27f, 0.15f);
        pyramid.scale = glm::vec3(0.3f, 0.3f, 0.3f);

        //Compilar shaders
        ShaderProgram myFirstProgram;
        myFirstProgram.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
        myFirstProgram.geometryShader = LoadGeometryShader("MyFirstGeometryShader.glsl");
        myFirstProgram.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");

        compiledPrograms.push_back(CreateProgram(myFirstProgram));

        glClearColor(0.f, 0.f, 0.f, 1.f);

        GLuint vaoCube, vboCube;

        glGenVertexArrays(1, &vaoCube);
        glBindVertexArray(vaoCube);

        glGenBuffers(1, &vboCube);
        glBindBuffer(GL_ARRAY_BUFFER, vboCube);

        GLfloat cubeVertices[] =
        {
            -0.5f, +0.5f, -0.5f,  
            +0.5f, +0.5f, -0.5f,  
            -0.5f, -0.5f, -0.5f,  
            +0.5f, -0.5f, -0.5f,  
            +0.5f, -0.5f, +0.5f,  
            +0.5f, +0.5f, -0.5f,  
            +0.5f, +0.5f, +0.5f,  
            -0.5f, +0.5f, -0.5f,  
            -0.5f, +0.5f, +0.5f,  
            -0.5f, -0.5f, -0.5f,  
            -0.5f, -0.5f, +0.5f,  
            +0.5f, -0.5f, +0.5f,  
            -0.5f, +0.5f, +0.5f,  
            +0.5f, +0.5f, +0.5f,  
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        GLuint vaoOrtho, vboOrtho;

        glGenVertexArrays(1, &vaoOrtho);
        glBindVertexArray(vaoOrtho);

        glGenBuffers(1, &vboOrtho);
        glBindBuffer(GL_ARRAY_BUFFER, vboOrtho);

        GLfloat orthoVertices[] =
        {
            -0.5f, +0.5f, -0.5f,
            +0.5f, +0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            +0.5f, -0.5f, -0.5f,
            +0.5f, -0.5f, +0.5f,
            +0.5f, +0.5f, -0.5f,
            +0.5f, +0.5f, +0.5f,
            -0.5f, +0.5f, -0.5f,
            -0.5f, +0.5f, +0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, +0.5f,
            +0.5f, -0.5f, +0.5f,
            -0.5f, +0.5f, +0.5f,
            +0.5f, +0.5f, +0.5f,
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(orthoVertices), orthoVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        GLuint vaoPyramid, vboPyramid;

        glGenVertexArrays(1, &vaoPyramid);
        glBindVertexArray(vaoPyramid);

        glGenBuffers(1, &vboPyramid);
        glBindBuffer(GL_ARRAY_BUFFER, vboPyramid);

        GLfloat pyramidVertices[] =
        {
             0.0f, +0.5f,  0.0f, 
            -0.5f, -0.5f, -0.5f,  
            +0.5f, -0.5f, -0.5f,  
             0.0f, +0.5f,  0.0f,  
            +0.5f, -0.5f, +0.5f,  
             0.0f, +0.5f,  0.0f,  
            -0.5f, -0.5f, +0.5f,  
            -0.5f, -0.5f, -0.5f,  
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        //Definimos modo wireframe para ver la estructura 3D
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //Indicar a la tarjeta GPU que programa debe usar
        glUseProgram(compiledPrograms[0]);

        //Asignar valores iniciales al programa
        glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

        //Generamos el game loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            //Dibujar cubo
            glm::mat4 cubeModelMatrix = glm::mat4(1.0f);

            cube.position = cube.position + cube.forward * cube.fVelocity;
            cube.rotation = cube.rotation + glm::vec3(0.f, 1.f, 0.f) * cube.fAngularVel;

            if(cube.position.y >= 0.7f || cube.position.y <= -0.7f){
                cube.forward = cube.forward * -1.f;
            }



            glm::mat4 cubeRotationMatrix = GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), cube.rotation.y);
            glm::mat4 cubeTranslationMatrix = GenerateTranslationMatrix(cube.position);
            glm::mat4 cubeScaleMatrix = GenerateScaleMatrix(cube.scale);

            //Aplicamos las matrices
            cubeModelMatrix = cubeTranslationMatrix * cubeRotationMatrix * cubeScaleMatrix * cubeModelMatrix;

            //Pasamos la matrix al shader
            glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(cubeModelMatrix));
            glUniform1i(glGetUniformLocation(compiledPrograms[0], "objectID"), 0);
            glBindVertexArray(vaoCube);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
            glBindVertexArray(0);

            //Dibujar ortoedro
            glm::mat4 orthoModelMatrix = glm::mat4(1.0f);

            glm::mat4 orthoTranslationMatrix = GenerateTranslationMatrix(ortho.position);
            glm::mat4 orthoScaleMatrix = GenerateScaleMatrix(ortho.scale);

            //Aplicamos las matrices
            orthoModelMatrix = orthoTranslationMatrix * orthoScaleMatrix;

            //Pasamos la matrix al shader
            glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(orthoModelMatrix));
            glUniform1i(glGetUniformLocation(compiledPrograms[0], "objectID"), 1);
            glBindVertexArray(vaoOrtho);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
            glBindVertexArray(0);

            //Dibujar piramide
            glm::mat4 pyramidModelMatrix = glm::mat4(1.0f);
            glm::mat4 pyramidTranslationMatrix = GenerateTranslationMatrix(pyramid.position);
            glm::mat4 pyramidScaleMatrix = GenerateScaleMatrix(pyramid.scale);

            //Aplicamos las matrices
            pyramidModelMatrix = pyramidTranslationMatrix * pyramidScaleMatrix;

            //Pasamos la matrix al shader
            glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(pyramidModelMatrix));
            glUniform1i(glGetUniformLocation(compiledPrograms[0], "objectID"), 2);
            glBindVertexArray(vaoPyramid);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
            glBindVertexArray(0);

            glFlush();
            glfwSwapBuffers(window);
        }

        //Desactivar y eliminar programa
        glUseProgram(0);
        glDeleteProgram(compiledPrograms[0]);
    }
    else
    {
        std::cout << "Ha petao." << std::endl;
        glfwTerminate();
    }

    glfwTerminate();
}