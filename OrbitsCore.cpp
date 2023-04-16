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
			}
			break;
		case SDL_MOUSEWHEEL:

			break;
		case SDL_MOUSEMOTION:
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LEFT)
			{

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
		planet->move(dt, planets);
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
	loadThread.done = true;
	return 0;
}