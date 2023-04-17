#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>

#include <SDL.h>

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
	inline SDL_Texture* planetTexture{};
	inline SDL_Point mouse{};
	inline SDL_Point savedPos{};
	inline Thread loadThread{};

	inline double prevTime{};
	inline double currTime{};
	inline bool running{};
	inline int focus{};
	inline int timeWarp{ 1 };
	inline Planet* planets[total_planets]{};

	void init(const char*);
	int load(void*);
	void handleEvents();
	void draw();
	void quit();
}

#endif