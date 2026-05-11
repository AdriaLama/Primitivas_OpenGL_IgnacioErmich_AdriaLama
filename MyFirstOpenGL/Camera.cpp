#include "Camera.h"
#include "RenderManager.h"

void Camera::Update(double dt)
{
	GLFWwindow* window = RenderManager::GetInstance()->GetWindow();

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		fFov += dt * 50;
		if (fFov > 100)
		{
			fFov = 100.f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		fFov -= dt * 50;
		if (fFov < 1)
		{
			fFov = 1.f;
		}
	}
	bool currentKeyC = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
	if (currentKeyC && !prevKeyC)
		isPerspectiveActive = !isPerspectiveActive;
	prevKeyC = currentKeyC;
}

glm::mat4 Camera::GetViewMatrix()
{
	const float radius = 3.f;
	float camX = sin(glfwGetTime()) * radius;
	float camY = 1.25f;
	float camZ = cos(glfwGetTime()) * radius;	
	return glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.f), localvectorUp);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	if (isPerspectiveActive)
		return glm::perspective(glm::radians(fFov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, fNear, fFar);

	else
		return glm::ortho(-1.5f, 1.f, -1.f, 1.f, fNear, fFar);
}