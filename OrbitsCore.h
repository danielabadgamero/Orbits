#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>
#include <string>

#include <SDL.h>
#include <SDL_net.h>

#include "Planet.h"
#include "Camera.h"

constexpr int totalPlanets{ 21 };

namespace Orbits
{
	inline std::vector<std::string> planetNames
	{
		"sun",
		"mercury",
		"venus",
		"earth",
		"moon",
		"mars",
		"phobos",
		"deimos",
		"jupiter",
		"io",
		"europa",
		"ganymede",
		"callisto",
		"saturn",
		"uranus",
		"neptune",
		"ceres",
		"eris",
		"haumea",
		"makemake",
		"pluto"
	};

	struct Thread
	{
		bool done{};
		SDL_Thread* thread{};
	};

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode monitor{};
	inline SDL_Point mouse{};
	inline SDL_Point savedPos{};
	inline Thread planetLoadThread{};
	inline IPaddress ip{};
	inline TCPsocket socket{};
	inline Thread imageLoadThread{};
	inline SDL_Surface* images[totalPlanets]{};

	inline double prevTime{};
	inline double currTime{};
	inline bool running{};
	inline int timeWarp{ 1 };
	inline Planet* planets[totalPlanets]{};

	void init(const char*);
	int loadPlanets(void*);
	int loadImages(void*);
	void handleEvents();
	void draw();
	void quit();
}

#endif