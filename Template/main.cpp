#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderProgram/ShaderProgram.h>

using std::cout;
using std::endl;

void initGlfw();
GLFWwindow* createWindow(std::ofstream& logger);
void handleInput(GLFWwindow* window);

int main()
{
	std::ofstream logger("logs.txt");

	initGlfw();
	GLFWwindow* main_window = createWindow(logger);
	if (!main_window) {
		return -1;
	}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logger << "main.cpp::Failed to load GLAD" << endl;
		return -1;
	}
	glViewport(0, 0, 1600, 900);
	/*
		CODE HERE!
	*/
	while (!glfwWindowShouldClose(main_window)) {
		handleInput(main_window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(main_window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

void initGlfw()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* createWindow(std::ofstream& logger)
{
	GLFWwindow* main_window = glfwCreateWindow(1600, 900, "Main window", 0, 0);
	if (main_window == 0) {
		logger << "main.cpp::Failed to create window" << endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(main_window);
	glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback);
	return main_window;
}

void handleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

