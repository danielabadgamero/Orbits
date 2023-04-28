#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Planet.h"

namespace Core
{
	inline struct
	{
		double dist{ 5e10 };
		double pitch{};
		double yaw{};
		int focus{};
		double speed{ 1 };
		double fov{ 90 };
	} camera{};

	inline GLFWwindow* window{};
	inline GLFWvidmode* screen{};

	inline std::chrono::high_resolution_clock::time_point prevTime;
	inline std::chrono::high_resolution_clock::time_point currTime;
	inline bool running{};
	inline std::vector<Planet*> planets{};

	void init(const char*);
	void handleEvents();
	void draw();
	void quit();
}

#endif