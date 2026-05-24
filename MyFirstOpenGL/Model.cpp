#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>  

#include <glm.hpp>

Model::Model(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals)
{
    numVertexs = static_cast<unsigned int>(vertexs.size()) / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &uvVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(float), vertexs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::Render() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
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
            struct FaceVertex { int v = 0, vt = 0, vn = 0; };
            std::vector<FaceVertex> faceVerts;

            std::string token;
            while (ss >> token)
            {
                FaceVertex fv;
                std::replace(token.begin(), token.end(), '/', ' ');
                std::stringstream ts(token);
                ts >> fv.v;
                ts >> fv.vt;
                ts >> fv.vn;
                faceVerts.push_back(fv);
            }

            auto addVertex = [&](const FaceVertex& fv)
                {
                    vertexs.push_back(tmpVertexs[(fv.v - 1) * 3]);
                    vertexs.push_back(tmpVertexs[(fv.v - 1) * 3 + 1]);
                    vertexs.push_back(tmpVertexs[(fv.v - 1) * 3 + 2]);

                    if (fv.vt > 0 && !tmpTextureCoordinates.empty())
                    {
                        textureCoordinates.push_back(tmpTextureCoordinates[(fv.vt - 1) * 2]);
                        textureCoordinates.push_back(tmpTextureCoordinates[(fv.vt - 1) * 2 + 1]);
                    }
                    else
                    {
                        textureCoordinates.push_back(0.f);
                        textureCoordinates.push_back(0.f);
                    }
       
                    if (fv.vn > 0 && !tmpNormals.empty())
                    {
                        vertexNormal.push_back(tmpNormals[(fv.vn - 1) * 3]);
                        vertexNormal.push_back(tmpNormals[(fv.vn - 1) * 3 + 1]);
                        vertexNormal.push_back(tmpNormals[(fv.vn - 1) * 3 + 2]);
                    }
                };

            for (size_t i = 1; i + 1 < faceVerts.size(); i++)
            {
                addVertex(faceVerts[0]);
                addVertex(faceVerts[i]);
                addVertex(faceVerts[i + 1]);
            }
        }
    }

    file.close();
    return Model(vertexs, textureCoordinates, vertexNormal);
}