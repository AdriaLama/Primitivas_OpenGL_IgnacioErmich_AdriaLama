#include "Camera.h"
#include "RenderManager.h"

void Camera::Update(double dt)
{
	GLFWwindow* window = RenderManager::GetInstance()->GetWindow();

	//verificar inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position.y += dt;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position.y -= dt;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position.x -= dt;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position.x += dt;

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) position.z += dt;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) position.z -= dt;

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
	//Genero la matriu de vista
	return glm::lookAt(position, position + glm::vec3(0.f, 0.f, -1.f), localvectorUp);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	if (isPerspectiveActive)
		return glm::perspective(glm::radians(fFov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, fNear, fFar);

	else
		return glm::ortho(-1.5f, 1.f, -1.f, 1.f, fNear, fFar);
}