#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>

class Model
{
public:

    Model(const std::vector<float>& vertexs,
        const std::vector<float>& uvs,
        const std::vector<float>& normals);

    // Binds VAO and draws
    void Render() const;
    GLuint textureID = 0;

private:
    GLuint VAO = 0;
    GLuint VBO = 0;    
    GLuint uvVBO = 0;    
    GLuint normalsVBO = 0;
    unsigned int numVertexs = 0;
};


Model LoadOBJModel(const std::string& filePath);