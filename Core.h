#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>
#include <chrono>

#include "Planet.h"

namespace Orbits
{
	enum Planets
	{
		sun,
		mercury,
		venus,
		earth,
		moon,
		mars,
		phobos,
		deimos,
		jupiter,
		io,
		europa,
		ganeymede,
		callisto,
		saturn,

		total_planets
	};

	inline struct
	{
		double dist{ 5e10 };
		double pitch{};
		double yaw{};
		int focus{};
		double speed{ 1 };
		double fov{ 90 };
	} camera{};

	inline std::chrono::high_resolution_clock::time_point prevTime;
	inline std::chrono::high_resolution_clock::time_point currTime;
	inline bool running{};
	inline int focus{};
	inline int timeWarp{ 1 };
	inline Planet* planets[total_planets]{};

	void init(const char*);
	void handleEvents();
	void draw();
	void quit();
}

#endif