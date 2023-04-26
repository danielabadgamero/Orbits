#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"

void Orbits::init(const char* title)
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

	running = true;
}

void Orbits::handleEvents()
{
	while (running)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Orbits::draw()
{
	prevTime = currTime;
	currTime = std::chrono::high_resolution_clock::now();
	double dt{ static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(currTime - prevTime).count()) };

	for (Planet*& planet : planets)
		if (planet)
			planet->move(dt);
}

void Orbits::quit()
{
	glfwTerminate();
}