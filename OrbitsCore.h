#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>
#include <string>

#include <SDL.h>
#include <SDL_net.h>

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
		ganymede,
		callisto,
		saturn,
		uranus,
		neptune,

		total_planets
	};

	struct Thread
	{
		bool done{};
		SDL_Thread* thread{};
	};

	inline struct
	{
		SDL_FPoint offset{};
		double zoomSpeed{ 1 };
		double zoom{ 1e-10 };
	} camera{};

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode monitor{};
	inline SDL_Point mouse{};
	inline SDL_Point savedPos{};
	inline Thread planetLoadThread{};
	inline IPaddress ip{};
	inline TCPsocket socket{};
	inline Thread imageLoadThread{};
	inline SDL_Surface* images[total_planets]{};

	inline double prevTime{};
	inline double currTime{};
	inline bool running{};
	inline int focus{};
	inline int timeWarp{ 1 };
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
		"neptune"
	};
	inline Planet* planets[total_planets]{};

	void init(const char*);
	int loadPlanets(void*);
	int loadImages(void*);
	void handleEvents();
	void draw();
	void quit();
}

#endif