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

	pixeldancer::Shader shockShader("shader/shockwave/shockwave.vs", "shader/shockwave/shockwave.ps");

	// variables
	glm::mat4 view_matrix, model_matrix;

	glm::vec3 lightPos = glm::vec3(-5.0f, 10.0f, -3.0f);

	glfwSetFramebufferSizeCallback(app.window, framebuffer_size_callback);
	// glfwSetCursorPosCallback(app.window, mouse_callback);
	// glfwSetScrollCallback(app.window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width, height;
	glfwGetFramebufferSize(app.window, &width, &height);
	proj_matrix = glm::perspective(glm::radians(cam.fov), (float)width / (float)height, 0.1f, 1000.0f);
	glViewport(0, 0, width, height);

	app.guiInit();

	std::string texFile = "data/textures/distortTexture.jpg";
	int texWidth, texHeight;
	unsigned int distortTex = pixeldancer::loadTex2D(texFile, &texWidth, &texHeight);

	model_matrix = glm::mat4(1.0f);

	glm::vec3 shockColor = glm::vec3(1.0f, 0.0f, 0.0f);

	float tempColor[3] = { 1.0f, 0.0f, 0.0f };

	float speed = 0.5f;
	float offset = 0.2f;
	float distort = 0.5f;
	float emissiveBooster = 2.0f;
	float edgeBoost = 50.0f;

	float time;

	while (!glfwWindowShouldClose(app.window))
	{
		cam.activateTimer();

		app.executeInput(cam);

		time = glfwGetTime() * speed;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		app.guiFrame();

		view_matrix = cam.getViewMatrix();

		shockShader.load();
		shockShader.setMat4("model_matrix", model_matrix);
		shockShader.setMat4("view_matrix", view_matrix);
		shockShader.setMat4("proj_matrix", proj_matrix);

		shockShader.setVec3("lightPos", lightPos);
		shockShader.setVec3("viewPos", cam.cameraPos);
		shockShader.setVec3("shockColor", shockColor);

		shockShader.setFloat("offset", offset);
		shockShader.setFloat("distort", distort);
		shockShader.setFloat("time", time);
		shockShader.setFloat("emissiveBooster", emissiveBooster);
		shockShader.setFloat("edgeBoost", edgeBoost);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, distortTex);

		factory.renderSphere();

		glUseProgram(0);

		ImGui::Begin("Shader Editor");
		ImGui::ColorEdit3("Shockwave Color", tempColor);
		ImGui::SliderFloat("Speed", &speed, 0.0f, 1.0f);
		ImGui::SliderFloat("Emissive Booster", &emissiveBooster, 2.0f, 10.0f);
		ImGui::SliderFloat("Edge Boost", &edgeBoost, 25.0f, 75.0f);
		ImGui::SliderFloat("Offset", &offset, -1.0f, 1.0f);
		ImGui::SliderFloat("Distort", &distort, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("Distort Texture");
		ImGui::Text("File: %s", texFile.c_str());
		ImGui::Text("Size: %d x %d", texWidth, texHeight);
		ImGui::Image((void*)(intptr_t)distortTex, ImVec2(texWidth, texHeight));
		ImGui::End();

		app.guiRender();

		shockColor.x = tempColor[0];
		shockColor.y = tempColor[1];
		shockColor.z = tempColor[2];

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