#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "OrbitsCore.h"

void Orbits::init(const char* title)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_GetCurrentDisplayMode(0, &monitor);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, monitor.w, monitor.h, SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	loadThread.thread = SDL_CreateThread(load, "loadScreens", NULL);
	while (!loadThread.done)
	{
		handleEvents();
		draw();
	}
	SDL_WaitThread(loadThread.thread, NULL);

	planetTexture = IMG_LoadTexture(renderer, "ball.png");

	running = true;
}

void Orbits::handleEvents()
{
	SDL_Event e;
	int x;
	int y;
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
			case SDL_SCANCODE_SPACE:
				if (focus + 1 == (int)planets.size())
					focus = 0;
				else
					focus++;
				break;
			case SDL_SCANCODE_PERIOD:
				if (timeWarp < 1000000)
				timeWarp *= 10;
				break;
			case SDL_SCANCODE_COMMA:
				if (timeWarp > 9)
					timeWarp /= 10;
			}
			break;
		case SDL_MOUSEWHEEL:
			SDL_GetMouseState(&x, &y);
			if (e.wheel.y > 0)
			{
				camera.zoom *= 2;
				camera.offset.x *= 2;
				camera.offset.y *= 2;
				camera.offset.x += x;
				camera.offset.y += y;
			}
			else
			{
				camera.zoom /= 2;
				camera.offset.x -= x;
				camera.offset.y -= y;
				camera.offset.x /= 2;
				camera.offset.y /= 2;
			}
			break;
		case SDL_MOUSEMOTION:
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LEFT)
			{
				camera.offset.x -= e.motion.xrel;
				camera.offset.y -= e.motion.yrel;
			}
			break;
		}
}

void Orbits::draw()
{
	prevTime = currTime;
	currTime = static_cast<double>(SDL_GetTicks64()) / 1000.0;
	double dt{ currTime - prevTime };

	SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xff);
	SDL_RenderClear(renderer);

	for (std::vector<Planet>::iterator planet{ planets.begin() }; planet != planets.end(); planet++)
	{
		if (dt < 0.02)
			planet->move(dt * timeWarp, planets);
		if (focus == std::distance(planets.begin(), planet) && focus)
			camera.offset = { planet->getPos(camera.zoom).x - monitor.w / 2, planet->getPos(camera.zoom).y - monitor.h / 2 };
		planet->draw(renderer, planetTexture, camera.zoom, camera.offset);
	}

	SDL_RenderPresent(renderer);
}

void Orbits::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int Orbits::load(void*)
{
	// Sun
	planets.push_back(Planet{ 1.989e30, 696340000, 0, 0, { 0xfd, 0xb8, 0x13 } });

	// Earth
	planets.push_back(Planet{ 5.972e24, 6371000, 152.1e9, 29.29e3, { 0x4f, 0x4c, 0xb0 } });
	// Moon
	planets.push_back(Planet{ 0.07346e24, 1736000, 0.4055e9 + 152.1e9, 0.97e3 + 29.29e3, { 0xb8, 0xae, 0xa3 } });

	// Jupiter
	planets.push_back(Planet{ 1898.13e24, 71492000, 816.363e9, 12.44e3, { 0xac, 0x81, 0x81 } });

	loadThread.done = true;
	return 0;
}