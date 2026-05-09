#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm.hpp>

Model::Model(const std::vector<float>& vertexs,
    const std::vector<float>& uvs,
    const std::vector<float>& normals)
{

    numVertexs = static_cast<unsigned int>(vertexs.size()) / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &uvVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,vertexs.size() * sizeof(float),vertexs.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glBufferData(GL_ARRAY_BUFFER,uvs.size() * sizeof(float),uvs.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::Render() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, numVertexs);
    glBindVertexArray(0);
}

Model LoadOBJModel(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }


    std::string line;
    std::stringstream ss;
    std::string prefix;
    glm::vec3 tmpVec3;  
    glm::vec2 tmpVec2;  


    std::vector<float> tmpVertexs;
    std::vector<float> tmpNormals;
    std::vector<float> tmpTextureCoordinates;


    std::vector<float> vertexs;
    std::vector<float> vertexNormal;
    std::vector<float> textureCoordinates;

    while (std::getline(file, line))
    {
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v")
        {
            ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;
            tmpVertexs.push_back(tmpVec3.x);
            tmpVertexs.push_back(tmpVec3.y);
            tmpVertexs.push_back(tmpVec3.z);
        }
        else if (prefix == "vt")
        {
            ss >> tmpVec2.x >> tmpVec2.y;
            tmpTextureCoordinates.push_back(tmpVec2.x);
            tmpTextureCoordinates.push_back(tmpVec2.y);
        }
        else if (prefix == "vn")
        {
            ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;
            tmpNormals.push_back(tmpVec3.x);
            tmpNormals.push_back(tmpVec3.y);
            tmpNormals.push_back(tmpVec3.z);
        }

        else if (prefix == "f")
        {
            int vertexData;
            short counter = 0;


            while (ss >> vertexData)
            {
                switch (counter)
                {
                case 0:
                    vertexs.push_back(tmpVertexs[((vertexData - 1) * 3)]);
                    vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 1]);
                    vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 2]);
                    ss.ignore(1, '/');
                    counter++;
                    break;

                case 1: 
                    textureCoordinates.push_back(tmpTextureCoordinates[((vertexData - 1) * 2)]);
                    textureCoordinates.push_back(tmpTextureCoordinates[((vertexData - 1) * 2) + 1]);
                    ss.ignore(1, '/');
                    counter++;
                    break;

                case 2:
                    vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3)]);
                    vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 1]);
                    vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 2]);
                    counter = 0;
                    break;
                }
            }
        }
    }

    file.close();
    return Model(vertexs, textureCoordinates, vertexNormal);
}