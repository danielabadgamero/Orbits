#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

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
	inline SDL_FPoint mouseDown{};
	inline SDL_Point mouse{};
	inline TTF_Font* font{};
	inline Thread loadThread{};
	inline std::vector<Planet> planets{};

	inline double prevTime{};
	inline double currTime{};
	inline bool running{};
	inline bool dragging{};

	inline int decPosition{};
	inline double values[5]{ 0, 5, 0xff, 0xff, 0xff };
	inline int selectedValue{};

	void init(const char*);
	int load(void*);
	void handleEvents();
	void draw();
	void quit();
}

#endif