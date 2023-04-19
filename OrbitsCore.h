#ifndef THUNDER_CORE
#define THUNDER_CORE

#include <vector>
#include <string>

#include <SDL.h>
#include <SDL_net.h>

#include "Planet.h"
#include "Camera.h"
#include "Vessel.h"

constexpr int totalPlanets{ 37 };

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
		"dione",
		"enceladus",
		"hyperion",
		"iapetus",
		"mimas",
		"rhea",
		"tethys",
		"titan",
		"uranus",
		"ariel",
		"miranda",
		"oberon",
		"puck",
		"titania",
		"umbriel",
		"neptune",
		"proteus",
		"triton",
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

	enum ViewMode
	{
		map,
		vessel,

		total_view_modes
	};

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode monitor{};
	inline SDL_Point mouse{};
	inline Thread planetLoadThread{};
	inline IPaddress ip{};
	inline TCPsocket socket{};
	inline Thread imageLoadThread{};
	inline SDL_Surface* images[totalPlanets]{};
	inline Planet* planets[totalPlanets]{};
	inline Camera camera{};
	inline ViewMode viewMode{ map };

	inline double prevTime{};
	inline double currTime{};
	inline bool running{};
	inline int timeWarp{};

	void init(const char*);
	int loadPlanets(void*);
	int loadImages(void*);
	void handleEvents();
	void draw();
	void quit();
}

#endif