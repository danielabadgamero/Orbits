#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Planet.h"

namespace Core
{
	inline GLFWwindow* window{};
	inline GLFWvidmode* screen{};

	inline std::chrono::high_resolution_clock::time_point prevTime;
	inline std::chrono::high_resolution_clock::time_point currTime;
	inline bool running{};
	inline std::vector<Planet*> planets{};

	constexpr inline float vertices[]
	{
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		 0.0f,  0.5f,  0.0f
	};

	inline GLuint VBO{};

	void init(const char*);
	void handleEvents();
	void draw();
	void quit();
}

#endif