#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>

#include <SDL.h>

#include "OrbitsWidget.h"

namespace Orbits
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode monitor{};

	class Screen
	{
	private:
		std::vector<Widget> widgets{};
	public:
		Screen(std::vector<Widget>);
		virtual void draw();
		virtual void event(SDL_Event*);
	};

	inline enum ScreenType
	{
		home,
	} currentScreen{};

	inline std::vector<Screen> screens{};

	inline bool running{};

	void init(const char*);
	void event();
	void draw();
	void quit();
}

#endif