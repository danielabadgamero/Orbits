#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>

#include <SDL.h>

#include "OrbitsScreen.h"

namespace Orbits
{
	enum ScreenType
	{
		home,
	};

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode monitor{};

	inline std::vector<Screen> screens{};

	inline bool running{};

	void init(const char*);
	void event();
	void draw();
	void quit();
}

#endif