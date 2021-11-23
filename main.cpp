#define STB_IMAGE_IMPLEMENTATION

#include "include/window.h"
#include "include/camera.h"
#include "include/shapeFactory.h"
#include "include/shader.h"

#include "include/texture.h"
#include "include/debug.h"
#include "include/model.h"

pixeldancer::Camera cam(WIDTH, HEIGHT);

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);

glm::mat4 proj_matrix;

int main()
{
	std::cout << "Default Width: " << WIDTH << " Default Height: " << HEIGHT << std::endl;

	pixeldancer::Application app;
	pixeldancer::ShapeFactory factory;

	glfwSetFramebufferSizeCallback(app.window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);

	int width, height;
	glfwGetFramebufferSize(app.window, &width, &height);
	proj_matrix = glm::perspective(glm::radians(cam.fov), (float)width / (float)height, 0.1f, 1000.0f);
	glViewport(0, 0, width, height);

	app.guiInit();
	
	bool isRendering = true;

	while (!glfwWindowShouldClose(app.window))
	{
		cam.activateTimer();

		// GUI 

		if (isRendering)
		{
			app.executeInput(cam);

			glfwSetCursorPosCallback(app.window, mouse_callback);
			glfwSetScrollCallback(app.window, scroll_callback);
			glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (glfwGetKey(app.window, GLFW_KEY_R) == GLFW_PRESS)
		{
			isRendering = !isRendering;

			glfwSetCursorPosCallback(app.window, NULL);
			glfwSetScrollCallback(app.window, NULL);
			glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		app.guiFrame();
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL

		// this is for rendering GUI
		glUseProgram(0);

		ImGui::Begin("Hello GUI");
		// gui codes here

		ImGui::End();

		app.guiRender();

		app.swap();
	}

	app.cleanup();

	exit(EXIT_SUCCESS);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	proj_matrix = glm::perspective(glm::radians(cam.fov), (float)width / (float)height, 0.1f, 1000.0f);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	cam.mouseInput(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.scrollInput(window, xoffset, yoffset);
}
