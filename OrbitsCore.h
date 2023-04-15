#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>

#include <SDL.h>

#include "Planet.h"

namespace Orbits
{
	struct Thread
	{
		bool done{};
		SDL_Thread* thread{};
	};

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode monitor{};
	inline SDL_Texture* planetTexture{};
	inline Thread loadThread{};

	inline double prevTime{};
	inline double currTime{};
	inline bool running{};
	inline int zoom{};
	inline std::vector<Planet> planets{};

	void init(const char*);
	int load(void*);
	void handleEvents();
	void draw();
	void quit();
}

#endif