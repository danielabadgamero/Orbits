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
			if (e.wheel.y > 0)
				camera.zoom /= 2;
			else
				camera.zoom *= 2;
			break;
		case SDL_MOUSEMOTION:
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LEFT)
			{
				camera.offset.x += e.motion.xrel;
				camera.offset.y += e.motion.yrel;
			}
			break;
		}
}

void Orbits::draw()
{
	prevTime = currTime;
	currTime = static_cast<double>(SDL_GetTicks64()) / 1000.0;

	SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xFF);
	SDL_RenderClear(renderer);

	for (Planet& planet : planets)
	{
		planet.move(currTime - prevTime, planets);
		planet.draw(renderer, planetTexture, camera.zoom, camera.offset);
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
	planets.push_back(Planet{ 1.989e30, 696340000, 0, 0, { 0xFD, 0xB8, 0x13 } });
	planets.push_back(Planet{ 5.972e24, 6371000, 152.1e9, 29.29e3, { 0xFD, 0xB8, 0x13 } });

	loadThread.done = true;
	return 0;
}