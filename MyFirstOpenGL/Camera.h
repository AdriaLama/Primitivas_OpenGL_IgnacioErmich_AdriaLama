#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:

	void Update(double dt);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	

private:

	glm::vec3 localvectorUp = glm::vec3(0.f, 1.f, 0.f);
	bool prevKeyC = false;
	bool isPerspectiveActive = true;
	float fFov = 45.f;
	float fNear = 0.1f;
	float fFar = 100.f;
	float fYaw = -90.f;  // Mirando hacia -Z por defecto
	float fPitch = -30.f;  // Inclinado hacia abajo
	
};
