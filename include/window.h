#pragma once

#include "camera.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
//	const char* message, const void* userParam)
//{
//	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
//
//	std::cout << "----------------------------------------------------------------------" << std::endl;
//	std::cout << "Debug Message (" << id << "): " << message << std::endl;
//
//	switch (source)
//	{
//	case GL_DEBUG_SOURCE_API: std::cout << "Source : API"; break;
//	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source : Window System"; break;
//	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source : Shader Compiler"; break;
//	case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source : Third Party"; break;
//	case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source : Application"; break;
//	case GL_DEBUG_SOURCE_OTHER: std::cout << "Source : Other"; break;
//	}
//	std::cout << std::endl;
//
//	switch (type)
//	{
//	case GL_DEBUG_TYPE_ERROR: std::cout << "Type : Error"; break;
//	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type : Depracted Behavior"; break;
//	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type : Undefined Behavior"; break;
//	case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type : Portability"; break;
//	case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type : Performance"; break;
//	case GL_DEBUG_TYPE_MARKER: std::cout << "Type : Marker"; break;
//	case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type : Pop Group"; break;
//	case GL_DEBUG_TYPE_OTHER: std::cout << "Type : Other"; break;
//	}
//	std::cout << std::endl;
//
//	switch (severity)
//	{
//	case GL_DEBUG_SEVERITY_HIGH: SHOWRED("Severity: HIGH"); break;
//	case GL_DEBUG_SEVERITY_MEDIUM: SHOWYELLOW("Severity: MEDIUM"); break;
//	case GL_DEBUG_SEVERITY_LOW: SHOWBLUE("Severity: LOW"); break;
//	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: NOTIFICATION\n"; break;
//	}
//	std::cout << std::endl;
//}

namespace pixeldancer
{
	class Application
	{
	public:
		Application(int width = WIDTH, int height = HEIGHT, bool debug = true, int _major = 4, int _minor = 3, std::string appName = "PixelDancer-OpenGL")
		{
			speed = 0.0f;
			accelerate = 1.0f;

			if (!glfwInit()) exit(EXIT_FAILURE);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _minor);

			/*if (debug)
			{
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
				int flags = 0;
				glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

				if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
				{
					glEnable(GL_DEBUG_OUTPUT);
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
					glDebugMessageCallback(glDebugOutput, nullptr);
					glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

					SHOWGREEN("Debug GENERATED");
				}
			}*/

			window = glfwCreateWindow(width, height, appName.c_str(), NULL, NULL);
			glfwMakeContextCurrent(window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) exit(EXIT_FAILURE);
		}

		void setVsync(bool vsync = true)
		{
			if (vsync) glfwSwapInterval(1);
		}

		void guiInit()
		{
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

			ImGui::StyleColorsDark();
		}

		void executeInput(Camera& cam, float givenSpeed = 10.0f)
		{
			if (accelerate < 1.0f)
				accelerate = 1.0f;

			speed = givenSpeed * cam.deltaTime * accelerate;

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				accelerate += 2.0f;

			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				accelerate -= 2.0f;

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				cam.cameraPos += speed * cam.cameraFront;

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				cam.cameraPos -= speed * cam.cameraFront;

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				cam.cameraPos -= glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp))
				* speed;

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				cam.cameraPos += glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp))
				* speed;
		}

		void guiFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void guiRender()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void swap()
		{
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		
		void cleanup()
		{
			glfwDestroyWindow(window);
			glfwTerminate();

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void getDetails()
		{
			const GLubyte* renderer = glGetString(GL_RENDERER);
			const GLubyte* vendor = glGetString(GL_VENDOR);
			const GLubyte* version = glGetString(GL_VERSION);
			const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

			GLint major, minor;
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			printf("GL Vendor : %s\n", vendor);
			printf("GL Renderer : %s\n", renderer);
			printf("GL Version (string) : %s\n", version);
			printf("GL Version (integer) : %d.%d\n", major, minor);
			printf("GLSL Version : %s\n", glslVersion);

			std::cout << "Extensions: " << std::endl;

			GLint nExtensions;
			glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);

			for (int i = 0; i < nExtensions; i++)
				printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
		}

	public:
		GLFWwindow* window;

	private:
		float speed, accelerate;
	};
}