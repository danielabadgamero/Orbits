#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>

#include <SDL.h>

#include "OrbitsWidget.h"

namespace Orbits
{
	class Screen
	{
	private:
		std::vector<Widget> widgets{};
	public:
		Screen(std::vector<Widget>);
		virtual void draw();
		virtual void handleEvent(SDL_Event*);
	};

	inline enum ScreenType
	{
		loading,
		home,
	} currentScreen{};

	struct Thread
	{
		bool done{};
		SDL_Thread* thread{};
	};

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode monitor{};
	inline Thread loadScreensThread{};

	inline std::vector<Screen> screens{};

	inline bool running{};

	void init(const char*);
	int loadScreens(void*);
	void event();
	void draw();
	void quit();
}

#endif