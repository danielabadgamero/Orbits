#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"

void Core::init(const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	screen = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());
	window = glfwCreateWindow(screen->width, screen->height, title, NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, 1600, 900);
	glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)([](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); }));
	glfwSetWindowCloseCallback(window, (GLFWwindowclosefun)([](GLFWwindow*) { running = false; }));

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	running = true;
}

void Core::handleEvents()
{
	while (running)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			running = false;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Core::draw()
{
	prevTime = currTime;
	currTime = std::chrono::high_resolution_clock::now();
	double dt{ static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(currTime - prevTime).count()) };

	for (Planet*& planet : planets)
		if (planet)
			planet->move(dt);
}

void Core::quit()
{
	glfwTerminate();
}