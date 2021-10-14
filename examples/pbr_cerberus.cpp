#define STB_IMAGE_IMPLEMENTATION

#include "include/window.h"
#include "include/camera.h"
#include "include/shapeFactory.h"
#include "include/shader.h"

#include "include/model.h"
#include "include/texture.h"
#include "include/light.h"
#include "include/material.h"
#include "include/pbr.h"
#include "include/debug.h"

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
	pixeldancer::PBR pbr;

	pixeldancer::Shader pbrShader("shader/pbr/pbr.vs", "shader/pbr/pbr.ps");
	pixeldancer::Shader cubemapShader("shader/pbr/cubemap.vs", "shader/pbr/cubemap.ps");

	pixeldancer::Model cerberus("data/objects/cerberus/CERBERUS_LP.FBX");

	// variables
	glm::mat4 view_matrix, model_matrix, MV_matrix;

	glfwSetFramebufferSizeCallback(app.window, framebuffer_size_callback);
	glfwSetCursorPosCallback(app.window, mouse_callback);
	glfwSetScrollCallback(app.window, scroll_callback);
	glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	unsigned int albedo = pixeldancer::loadTex2D("data/objects/cerberus/Textures/CERBERUS_A.tga");
	unsigned int normal = pixeldancer::loadTex2D("data/objects/cerberus/Textures/CERBERUS_N.tga");
	unsigned int metallic = pixeldancer::loadTex2D("data/objects/cerberus/Textures/CERBERUS_M.tga");
	unsigned int roughness = pixeldancer::loadTex2D("data/objects/cerberus/Textures/CERBERUS_R.tga");
	unsigned int ao = pixeldancer::loadTex2D("data/objects/cerberus/Textures/Raw/CERBERUS_AO.tga");

	pbr.initializeWPreShaders("data/textures/hdr/newport_loft.hdr");

	pbr.setPBRTextures(pbrShader, albedo, normal, metallic, roughness, ao);

	glm::vec3 lightPositions[4] =
	{
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f)
	};

	glm::vec3 lightColors[4] =
	{
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model_matrix = glm::rotate(model_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));

	int width, height;
	glfwGetFramebufferSize(app.window, &width, &height);
	proj_matrix = glm::perspective(glm::radians(cam.fov), (float)width / (float)height, 0.1f, 1000.0f);
	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(app.window))
	{
		cam.activateTimer();

		app.executeInput(cam);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view_matrix = cam.getViewMatrix();

		pbrShader.load();
		pbrShader.setMat4("model_matrix", model_matrix);
		pbrShader.setMat4("view_matrix", view_matrix);
		pbrShader.setMat4("proj_matrix", proj_matrix);
		pbrShader.setVec3("cameraPos", cam.cameraPos);

		for (unsigned int i = 0; i < 4; i++)
		{
			glm::vec3 newPos = lightPositions[i];
			
			pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
		}

		// factory.renderCube();
		cerberus.draw(pbrShader);

		// ////////////////////////////////////////////////////////////////////////////////////////////////////
		// render cubemap
		// ////////////////////////////////////////////////////////////////////////////////////////////////////

		cubemapShader.load();
		cubemapShader.setMat4("proj_matrix", proj_matrix);
		cubemapShader.setMat4("view_matrix", view_matrix);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pbr.envCubemap);

		factory.renderCube();

		app.swap();
	}

	app.cleanup();
	factory.cleanup();
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