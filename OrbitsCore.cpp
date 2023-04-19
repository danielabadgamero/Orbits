#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>

#include "OrbitsCore.h"
#include "Planet.h"
#include "Camera.h"

static int index(std::string name)
{
	return static_cast<int>(std::distance(Orbits::planetNames.begin(), std::find(Orbits::planetNames.begin(), Orbits::planetNames.end(), name)));
}

void Orbits::init(const char* title)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	SDLNet_Init();

	SDLNet_ResolveHost(&ip, "w.solarsystemscope.com", 80);

	SDL_GetCurrentDisplayMode(0, &monitor);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, monitor.w, monitor.h, SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	TTF_Font* font{ TTF_OpenFont("C:\\Windows\\Fonts\\AGENCYR.TTF", 32) };
	int progress{};

	planetLoadThread.thread = SDL_CreateThread(loadPlanets, "loadPlanets", NULL);
	imageLoadThread.thread = SDL_CreateThread(loadImages, "loadImages", NULL);
	while (!planetLoadThread.done || !imageLoadThread.done)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e));
		if (images[progress])
			if (progress + 1 < static_cast<int>(planetNames.size()))
				progress++;
		std::string msg{ "Loading " + planetNames[progress] };
		SDL_Surface* textSurface{ TTF_RenderText_Blended(font, msg.c_str(), { 0xff, 0x30, 0x30, 0xff }) };
		SDL_Texture* renderTexture{ SDL_CreateTextureFromSurface(renderer, textSurface) };
		SDL_Rect rect{ 20, monitor.h - 20 };
		SDL_QueryTexture(renderTexture, NULL, NULL, &rect.w, &rect.h);
		rect.y -= rect.h;

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, renderTexture, NULL, &rect);
		SDL_RenderPresent(renderer);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(renderTexture);
	}
	SDL_WaitThread(planetLoadThread.thread, NULL);
	SDL_WaitThread(imageLoadThread.thread, NULL);
	TTF_CloseFont(font);

	double initWidth{ 1e12 };
	camera =
	{{
		static_cast<float>(initWidth),
		static_cast<float>(initWidth / monitor.w * monitor.h)
	}};

	timeWarp = 1;

	running = true;
}

void Orbits::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.scancode)
			{
			case SDL_SCANCODE_ESCAPE:
				running = false;
				break;
			case SDL_SCANCODE_UP:
				if (camera.getFocus() == totalPlanets - 1)
					camera.getFocus() = 0;
				else
					camera.getFocus()++;
				break;
			case SDL_SCANCODE_DOWN:
				if (camera.getFocus() == 0)
					camera.getFocus() = totalPlanets - 1;
				else
					camera.getFocus()--;
				break;
			case SDL_SCANCODE_SPACE:
				camera.getFocus() = 0;
				break;
			case SDL_SCANCODE_PERIOD:
				if (timeWarp < 1000000000)
					timeWarp *= 10;
				break;
			case SDL_SCANCODE_COMMA:
				if (timeWarp > 9)
					timeWarp /= 10;
				break;
			case SDL_SCANCODE_TAB:
				if (SDL_GetKeyboardState(NULL)[SDLK_LSHIFT])
					if (viewMode == 0)
						viewMode = static_cast<ViewMode>(total_view_modes - 1);
					else
						viewMode = static_cast<ViewMode>(viewMode - 1);
				else
					if (viewMode == total_view_modes - 1)
						viewMode = static_cast<ViewMode>(0);
					else
						viewMode = static_cast<ViewMode>(viewMode + 1);
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
			camera.savePos(mouse);
			if (e.wheel.y > 0)
				camera.getZoomFactor() /= 1.1;
			else
				camera.getZoomFactor() *= 1.1;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse.x, &mouse.y);
			if (camera.getFocus() != 0)
				break;
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LEFT)
				camera.move(e.motion.xrel, e.motion.yrel);
			break;
		}
}

void Orbits::draw()
{
	prevTime = currTime;
	currTime = static_cast<double>(SDL_GetTicks64()) / 1000.0;
	double dt{ currTime - prevTime };

	SDL_SetRenderDrawColor(renderer, 0x08, 0x08, 0x08, 0xff);
	SDL_RenderClear(renderer);

	switch (viewMode)
	{
	case map:
		if (camera.getZoomFactor() != 1)
			camera.zoom();
		for (int i{}; i != totalPlanets; i++)
			planets[i]->move(dt * timeWarp);
		if (camera.getFocus())
			camera.focus();
		for (int i{}; i != totalPlanets; i++)
			planets[i]->draw(images[i], camera.getViewport());
		break;
	case vessel:
		break;
	}

	SDL_RenderPresent(renderer);
}

void Orbits::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDLNet_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int Orbits::loadPlanets(void*)
{
	planets[index("sun")] = new Planet{ NULL, 1.989e30, 696340000, 0, 0, "sun" };
	planets[index("mercury")] = new Planet{ planets[index("sun")], 0.33010e24, 2440500, 57.909e9, 0.2056, "mercury" };
	planets[index("venus")] = new Planet{ planets[index("sun")], 4.8673e24, 6051800, 108.210e9, 0.0068, "venus" };
	planets[index("earth")] = new Planet{ planets[index("sun")], 5.9722e24, 6378137, 149.598e9, 0.0167, "earth" };
	planets[index("moon")] = new Planet{ planets[index("earth")], 0.07346e24, 1738100, 0.3844e9, 0.0549, "moon" };
	planets[index("mars")] = new Planet{ planets[index("sun")], 0.64169e24, 3396200, 227.956e9, 0.0935, "mars" };
	planets[index("phobos")] = new Planet{ planets[index("mars")], 10.6e15, 13000, 9378e3, 0.0151, "phobos" };
	planets[index("deimos")] = new Planet{ planets[index("mars")], 2.4e15, 7800, 23459e3, 0.0005, "deimos" };
	planets[index("jupiter")] = new Planet{ planets[index("sun")], 1898.13e24, 71492000, 778.479e9, 0.0487, "jupiter" };
	planets[index("io")] = new Planet{ planets[index("jupiter")], 893.2e20, 1821500, 421.8e6, 0.004, "io" };
	planets[index("europa")] = new Planet{ planets[index("jupiter")], 480e20, 1560800, 671.1e6, 0.009, "europa" };
	planets[index("ganymede")] = new Planet{ planets[index("jupiter")], 1481.9e20, 2631200, 1070.4e6, 0.001, "ganymede" };
	planets[index("callisto")] = new Planet{ planets[index("jupiter")], 1075.9e20, 2410300, 1882.7e6, 0.007, "callisto" };
	planets[index("saturn")] = new Planet{ planets[index("sun")], 568.32e24, 60268000, 1432.041e9, 0.0520, "saturn" };
	planets[index("dione")] = new Planet{ planets[index("saturn")], 11e20, 561000, 377.40e6, 0.0022, "dione" };
	planets[index("enceladus")] = new Planet{ planets[index("saturn")], 1.08e20, 561000, 238.02e6, 0.0022, "enceladus" };
	planets[index("hyperion")] = new Planet{ planets[index("saturn")], 5.5510e18, 135000, 1481009e3, 0.1230061, "hyperion" };
	planets[index("iapetus")] = new Planet{ planets[index("saturn")], 1.8056591e21, 734400, 3560820e3, 0.0276812, "iapetus" };
	planets[index("mimas")] = new Planet{ planets[index("saturn")], 3.75094e19, 198200, 185539e3, 0.0196, "mimas" };
	planets[index("rhea")] = new Planet{ planets[index("saturn")], 2.3064854e21, 763500, 527108e3, 0.0012583, "rhea" };
	planets[index("tethys")] = new Planet{ planets[index("saturn")], 6.174959e20, 531100, 294619e3, 0.0001, "tethys" };
	planets[index("titan")] = new Planet{ planets[index("saturn")], 1.3452e23, 2574730, 1221870e3, 0.0288, "titan" };
	planets[index("uranus")] = new Planet{ planets[index("sun")], 86.811e24, 25559000, 2867.043e9, 0.0469, "uranus" };
	planets[index("ariel")] = new Planet{ planets[index("uranus")], 1.251e21, 578900, 191020e3, 0.0012, "ariel" };
	planets[index("miranda")] = new Planet{ planets[index("uranus")], 6.4e19, 235800, 129390e3, 0.0013, "miranda" };
	planets[index("oberon")] = new Planet{ planets[index("uranus")], 3.076e21, 761400, 583520e3, 0.0014, "oberon" };
	planets[index("puck")] = new Planet{ planets[index("uranus")], 2.9e18, 81000, 86004.444e3, 0.00012, "puck" };
	planets[index("titania")] = new Planet{ planets[index("uranus")], 3.4e21, 788400, 435910e3, 0.0011, "titania" };
	planets[index("umbriel")] = new Planet{ planets[index("uranus")], 1.275e21, 584700, 266e6, 0.0039, "umbriel" };
	planets[index("neptune")] = new Planet{ planets[index("sun")], 102.409e24, 24764000, 4514.953e9, 0.0097, "neptune" };
	planets[index("proteus")] = new Planet{ planets[index("neptune")], 4.4e19, 210000, 117647e3, 0.00053, "proteus" };
	planets[index("triton")] = new Planet{ planets[index("neptune")], 2.139e22, 1353400, 354759e3, 0.000016, "triton" };
	planets[index("ceres")] = new Planet{ planets[index("sun")], 939300e15, 961000, 4.14087e11, 0.0758, "ceres" };
	planets[index("eris")] = new Planet{ planets[index("sun")], 1.6466e22, 1163000, 1.015231e13, 0.43607, "eris" };
	planets[index("haumea")] = new Planet{ planets[index("sun")], 4.006e22, 780000, 6.450062e12, 0.19642, "haumea" };
	planets[index("makemake")] = new Planet{ planets[index("sun")], 3.1e21, 715000, 6.79623e12, 0.16126, "makemake" };
	planets[index("pluto")] = new Planet{ planets[index("sun")], 0.01303e24, 1188000, 5869.656e9, 0.2444, "pluto" };

	planetLoadThread.done = true;
	return 0;
}

static std::vector<char> getImage(std::string planet)
{ retry:
	Orbits::socket = SDLNet_TCP_Open(&Orbits::ip);
	std::string request{ "GET /spacepedia/images/handbook/renders/" + planet + ".png HTTP/1.0\nHost: w.solarsystemscope.com\n\n" };
	SDLNet_TCP_Send(Orbits::socket, request.data(), static_cast<int>(request.size()) + 1);

	std::vector<char> response{};
	do response.push_back(0);
	while (SDLNet_TCP_Recv(Orbits::socket, &response.back(), 1));

	while (response.front() != -119)
	{
		response.erase(response.begin());
		if (response.empty())
		{
			SDLNet_TCP_Close(Orbits::socket);
			goto retry;
		}
	}

	SDLNet_TCP_Close(Orbits::socket);

	return response;
}

static void loadImage(std::string name)
{
	std::string path{ "images/" + name + ".png" };
	if (std::filesystem::exists(path))
		Orbits::images[index(name)] = IMG_Load(path.c_str());
	else
	{
		std::vector<char> img{ getImage(name) };
		SDL_RWops* data{ SDL_RWFromMem(img.data(), static_cast<int>(img.size())) };
		Orbits::images[index(name)] = IMG_LoadPNG_RW(data);
		IMG_SavePNG(Orbits::images[index(name)], path.c_str());
		SDL_RWclose(data);
	}
}

int Orbits::loadImages(void*)
{
	for (const std::string& planet : planetNames)
		loadImage(planet);

	imageLoadThread.done = true;
	return 0;
}