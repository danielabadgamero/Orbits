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
		earth,
		moon,

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
		double zoom{ 1 };
		double zoomSpeed{ 1 };
	} camera{};

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode monitor{};
	inline SDL_Texture* planetTexture{};
	inline SDL_Point mouse{};
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