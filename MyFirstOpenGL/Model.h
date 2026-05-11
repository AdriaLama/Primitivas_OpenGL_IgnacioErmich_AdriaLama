#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>

class Model
{
public:
    // Constructs the Model: creates VAO + VBO (positions) + uvVBO (UVs)
    Model(const std::vector<float>& vertexs,
        const std::vector<float>& uvs,
        const std::vector<float>& normals);

    // Binds VAO and draws
    void Render() const;
    GLuint textureID = 0;

private:
    GLuint VAO = 0;
    GLuint VBO = 0;    // layout = 0  (positions)
    GLuint uvVBO = 0;    // layout = 1  (texture coords)
    unsigned int numVertexs = 0;
};

// Parses an .obj file and returns a ready-to-render Model
Model LoadOBJModel(const std::string& filePath);