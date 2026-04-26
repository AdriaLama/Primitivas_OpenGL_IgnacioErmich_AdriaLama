#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

glm::mat4 GenerateTranslationMatrix(glm::vec3 translation);
glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees);
glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis);
