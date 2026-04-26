#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "GameObject.h"
#include "Shader.h"
#include "Transform.h"

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#define ID_CUBO 0
#define ID_ORTOEDRO 1
#define ID_PIRAMIDE 2

std::vector<GLuint> compiledPrograms;

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight)
{
    //Definir nuevo tamaño del viewport
    glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
    glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
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

        float tiempo = static_cast<float>(glfwGetTime());
        bool bPaused = false;
        bool bSpaceWasPressed = false;
        bool bMWasPressed = false;
        bool bNWasPressed = false;
        bool b1WasPressed = false;
        bool wireframe = false;
        bool b2WasPressed = false;
        bool b3WasPressed = false;
        bool b4WasPressed = false;



        bool showCube = true;
        bool showOrtho = true;
        bool showPyramid = true;

        //Declarar instancias de GameObject para cada figura
        GameObject cube;
        GameObject ortho;
        GameObject pyramid;

        cube.position = glm::vec3(-0.55f, 0.f, 0.f);
        ortho.position = glm::vec3(0.0f, 0.f, 0.f);
        pyramid.position = glm::vec3(0.55f, 0.f, 0.f);

        cube.forward = glm::vec3(0.f, 1.f, 0.f);
        pyramid.forward = glm::vec3(0.f, 1.f, 0.f);

        cube.scale = glm::vec3(0.3f, 0.3f, 0.3f);
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
            -0.5f, -0.5f, +0.5f,
            +0.5f, -0.5f, +0.5f,

             0.0f, +0.5f,  0.0f,
            +0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

             0.0f, +0.5f,  0.0f,
            +0.5f, -0.5f, +0.5f,
            +0.5f, -0.5f, -0.5f,

             0.0f, +0.5f,  0.0f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, +0.5f,

            -0.5f, -0.5f, -0.5f,
            +0.5f, -0.5f, -0.5f,
            +0.5f, -0.5f, +0.5f,
            -0.5f, -0.5f, -0.5f,
            +0.5f, -0.5f, +0.5f,
            -0.5f, -0.5f, +0.5f,
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        //Comprobacion de pulsamiento tecla 1

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        //Indicar a la tarjeta GPU que programa debe usar
        glUseProgram(compiledPrograms[0]);

        //Asignar valores iniciales al programa
        glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

        //Generamos el game loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            tiempo = static_cast<float>(glfwGetTime());
            bool b1IsPressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
            if (b1IsPressed && !b1WasPressed) {
                wireframe = !wireframe;
                glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

            }
            b1WasPressed = b1IsPressed;

            //Pausar y reanudar ejecucion programa
            bool bSpaceIsPressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
            if (bSpaceIsPressed && !bSpaceWasPressed) {
                bPaused = !bPaused;
            }
            bSpaceWasPressed = bSpaceIsPressed;

            if (bPaused) {
                glfwSwapBuffers(window);
                continue;
            }

            //Acelerar y reducir velocidades de transformaciones
            bool bMIsPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
            bool bNIsPressed = glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS;

            if (bMIsPressed && !bMWasPressed) {
                cube.fVelocity += cube.fVelocity * 0.10f;
                cube.fAngularVel += cube.fAngularVel * 0.10f;
                ortho.fScaleVel += ortho.fScaleVel * 0.10f;
                ortho.fAngularVel += ortho.fAngularVel * 0.10f;
                pyramid.fVelocity += pyramid.fVelocity * 0.10f;
                pyramid.fAngularVel += pyramid.fAngularVel * 0.10f;
            }

            if (bNIsPressed && !bNWasPressed) {
                cube.fVelocity -= cube.fVelocity * 0.10f;
                cube.fAngularVel -= cube.fAngularVel * 0.10f;
                ortho.fScaleVel -= ortho.fScaleVel * 0.10f;
                ortho.fAngularVel -= ortho.fAngularVel * 0.10f;
                pyramid.fVelocity -= pyramid.fVelocity * 0.10f;
                pyramid.fAngularVel -= pyramid.fAngularVel * 0.10f;
            }

            bMWasPressed = bMIsPressed;
            bNWasPressed = bNIsPressed;



            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            //Verificacion apretar tecla 2
            bool b2IsPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
            if (b2IsPressed && !b2WasPressed) {
                showCube = !showCube;
            }
            b2WasPressed = b2IsPressed;

            //Verificacion apretar tecla 3
            bool b3IsPressed = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
            if (b3IsPressed && !b3WasPressed) {
                showOrtho = !showOrtho;
            }
            b3WasPressed = b3IsPressed;

            //Verificacion apretar tecla 4
            bool b4IsPressed = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
            if (b4IsPressed && !b4WasPressed) {
                showPyramid = !showPyramid;
            }
            b4WasPressed = b4IsPressed;

            cube.position = cube.position + cube.forward * cube.fVelocity;
            cube.rotation = cube.rotation + glm::vec3(0.f, 1.f, 0.f) * cube.fAngularVel;

            //Movimiento cubo arriba y abajo
            if (cube.position.y >= 0.7f || cube.position.y <= -0.7f) {
                cube.forward = cube.forward * -1.f;
            }

            //Dibujar cubo dependiendo de showCube
            glm::mat4 cubeModelMatrix = glm::mat4(showCube ? 1.0f : 0.f);
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


            ortho.rotation = ortho.rotation + glm::vec3(0.f, 0.f, 1.f) * ortho.fAngularVel;
            ortho.scale = ortho.scale + glm::vec3(1.f, 0.f, 0.f) * ortho.fScaleVel;

            //Escalando continuamente el ortoedro
            if (ortho.scale.x >= 0.3f || ortho.scale.x <= 0.05f) {
                ortho.fScaleVel = ortho.fScaleVel * -1.f;
            }

            //Aplicamos las matrices y dibujamos en funcion de showOrtho
            glm::mat4 orthoModelMatrix = glm::mat4(showOrtho ? 1.0f : 0.f);
            glm::mat4 orthoRotationMatrix = GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), ortho.rotation.z);
            glm::mat4 orthoTranslationMatrix = GenerateTranslationMatrix(ortho.position);
            glm::mat4 orthoScaleMatrix = GenerateScaleMatrix(ortho.scale);
            orthoModelMatrix = orthoTranslationMatrix * orthoRotationMatrix * orthoScaleMatrix * orthoModelMatrix;


            //Pasamos la matrix al shader
            glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(orthoModelMatrix));
            glUniform1i(glGetUniformLocation(compiledPrograms[0], "objectID"), 1);
            glBindVertexArray(vaoOrtho);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
            glBindVertexArray(0);


            pyramid.position = pyramid.position + pyramid.forward * pyramid.fVelocity;
            pyramid.rotation = pyramid.rotation + glm::vec3(1.f, 1.f, 0.f) * pyramid.fAngularVel;

            if (pyramid.position.y >= 0.7f || pyramid.position.y <= -0.7f) {
                pyramid.forward = pyramid.forward * -1.f;
            }

            //Dibujar piramide dependiendo de showPyramid
            glm::mat4 pyramidModelMatrix = glm::mat4(showPyramid ? 1.0f : 0.f);
            glm::mat4 pyramidRotationMatrix = GenerateRotationMatrix(glm::vec3(1.f, 0.f, 0.f), pyramid.rotation.x) * GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), pyramid.rotation.y);
            glm::mat4 pyramidTranslationMatrix = GenerateTranslationMatrix(pyramid.position);
            glm::mat4 pyramidScaleMatrix = GenerateScaleMatrix(pyramid.scale);

            //Aplicamos las matrices
            pyramidModelMatrix = pyramidTranslationMatrix * pyramidRotationMatrix * pyramidScaleMatrix * pyramidModelMatrix;
            //Pasamos la matrix al shader
            glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(pyramidModelMatrix));
            glUniform1i(glGetUniformLocation(compiledPrograms[0], "objectID"), 2);
            glUniform1f(glGetUniformLocation(compiledPrograms[0], "tiempo"), tiempo);
            glBindVertexArray(vaoPyramid);
            glDrawArrays(GL_TRIANGLES, 0, 18);
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