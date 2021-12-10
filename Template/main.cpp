#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderProgram/ShaderProgram.h>
#include "Matrix.h"

using namespace std;

const int WIDTH = 1600, HEIGHT = 900;



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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* createWindow(ofstream& logger)
{
	GLFWwindow* main_window = glfwCreateWindow(WIDTH, HEIGHT, "Main window", glfwGetPrimaryMonitor(), 0);
	if (main_window == 0) {
		logger << "main.cpp::Failed to create window" << endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(main_window);
	glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback);
	glfwSetKeyCallback(main_window, key_callback);

	return main_window;
}

void createVertexBuffer(GLuint& VBO)
{
	Vector3f vertices[3];
	vertices[0] = Vector3f(-0.5f, -0.5f, 0.0f);
	vertices[1] = Vector3f(0.5f, -0.5f, 0.0f);
	vertices[2] = Vector3f(0.0f, 0.5f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void createArrayBuffer(GLuint& VAO)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void renderScene(GLFWwindow* window, GLuint& VBO, GLuint& gWorldLocation)
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;
	Scale += 0.001f;

	Matrix4f World;
    World.m[0][0] = 1.0f; World.m[0][1] = 0.0f; World.m[0][2] = 0.0f; World.m[0][3] = sinf(Scale);
    World.m[1][0] = 0.0f; World.m[1][1] = 1.0f; World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
    World.m[2][0] = 0.0f; World.m[2][1] = 0.0f; World.m[2][2] = 1.0f; World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f; World.m[3][1] = 0.0f; World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;
	
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(0);
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main()
{
	ofstream logger("logs.txt");
	initGlfw();
	// create window
	GLFWwindow* main_window = createWindow(logger);
	if (!main_window) {
		return -1;
	}

	// load functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logger << "main.cpp::Failed to load GLAD" << endl;
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint VBO, VAO;
	createVertexBuffer(VBO);
	createArrayBuffer(VAO);

	ShaderProgram shaderProgram("shader", "shader.vs", "shader.fs", logger);
	GLuint gWorldLocation = glGetUniformLocation(shaderProgram.getProgramId(), "gWorld");
	if (gWorldLocation == 0xFFFFFFFF) {
		logger << "FAILURE::Uniform not found" << endl;
	}
	shaderProgram.use();

	while (!glfwWindowShouldClose(main_window)) {
		renderScene(main_window, VBO, gWorldLocation);
		glfwPollEvents();
	}

    glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return 0;
}



