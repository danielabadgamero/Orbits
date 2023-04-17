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
				if (focus + 1 == total_planets)
					focus = 0;
				else
					focus++;
				break;
			case SDL_SCANCODE_PERIOD:
				if (timeWarp < 1000000000)
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
			if (focus)
				break;
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

	for (int i{}; i != total_planets; i++)
	{
		planets[i]->move(dt * timeWarp);
		if (focus == i && focus)
			camera.offset = { planets[i]->getPos(camera.zoom).x - monitor.w / 2, planets[i]->getPos(camera.zoom).y - monitor.h / 2 };
	}

	SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xff);
	SDL_RenderClear(renderer);

	for (int i{}; i != total_planets; i++)
		planets[i]->draw(renderer, planetTexture, camera.zoom, camera.offset);

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
	planets[sun]	= new Planet{ NULL, 1.989e30, 696340000, 0, 0, { 0xfd, 0xb8, 0x13 } };
	planets[earth]	= new Planet{ planets[sun], 5.9722e24, 6378137, 149.598e9, 0.0167, { 0x00, 0x00, 0xa5 } };
	planets[moon]	= new Planet{ planets[earth], 0.07346e24, 1738100, 0.3844e9, 0.0549, { 0xb8, 0xae, 0xa3 } };

	loadThread.done = true;
	return 0;
}