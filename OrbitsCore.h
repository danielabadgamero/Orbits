#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>

#include <SDL.h>

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
	inline Thread loadScreensThread{};

	inline bool running{};

	void init(const char*);
	int loadScreens(void*);
	void event();
	void draw();
	void quit();
}

#endif