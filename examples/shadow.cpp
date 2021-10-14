#define STB_IMAGE_IMPLEMENTATION

#include "include/window.h"
#include "include/camera.h"
#include "include/shapeFactory.h"
#include "include/shader.h"

#include "include/texture.h"
#include "include/debug.h"
#include "include/shadow.h"

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
	pixeldancer::ShadowMapping shadow;
	
	pixeldancer::Shader perspLight("shader/shadow/perspLight.vs", "shader/shadow/perspLight.ps");
	pixeldancer::Shader sceneShader("shader/shadow/shadow.vs", "shader/shadow/shadow.ps");

	pixeldancer::Shader debugShader("shader/debug/framebuffer.vs", "shader/debug/shadow.ps");

	// variables
	glm::mat4 view_matrix, model_matrix;

	glm::vec3 lightPos = glm::vec3(-5.0f, 10.0f, -3.0f);

	glfwSetFramebufferSizeCallback(app.window, framebuffer_size_callback);
	glfwSetCursorPosCallback(app.window, mouse_callback);
	glfwSetScrollCallback(app.window, scroll_callback);
	glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	// for peter panning (not real visible attached shadows)
	// they are moved because of shadow bias
	glEnable(GL_CULL_FACE);

	int width, height;
	glfwGetFramebufferSize(app.window, &width, &height);
	proj_matrix = glm::perspective(glm::radians(cam.fov), (float)width / (float)height, 0.1f, 1000.0f);
	glViewport(0, 0, width, height);

	unsigned int diffuseTex = pixeldancer::loadTex2D("data/textures/wood.png");

	shadow.initialize();
	glm::mat4 light_proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);
	glm::mat4 light_view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightMatrix = light_proj * light_view;

	while (!glfwWindowShouldClose(app.window))
	{
		cam.activateTimer();

		app.executeInput(cam);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, shadow.framebuffer_shadowMap);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);

		perspLight.load();
		perspLight.setMat4("lightMatrix", lightMatrix);

		model_matrix = glm::mat4(1.0f);
		perspLight.setMat4("model_matrix", model_matrix);
		factory.renderPlane();
		model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 3.0f, 0.0f));
		perspLight.setMat4("model_matrix", model_matrix);
		factory.renderSphere();

		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view_matrix = cam.getViewMatrix();

		sceneShader.load();
		
		sceneShader.setMat4("view_matrix", view_matrix);
		sceneShader.setMat4("proj_matrix", proj_matrix);
		sceneShader.setMat4("lightMatrix", lightMatrix);
		
		sceneShader.setVec3("lightPos", lightPos);
		sceneShader.setVec3("viewPos", cam.cameraPos);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadow.texture_shadowMap);

		model_matrix = glm::mat4(1.0f);
		sceneShader.setMat4("model_matrix", model_matrix);
		factory.renderPlane();
		model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 3.0f, 0.0f));
		sceneShader.setMat4("model_matrix", model_matrix);
		factory.renderSphere();

		debugTexture(shadow.texture_shadowMap, diffuseTex, debugShader, factory);

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