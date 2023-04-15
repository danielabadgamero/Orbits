#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "OrbitsCore.h"
#include "OrbitsWidget.h"

void Orbits::init(const char* title)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_GetCurrentDisplayMode(0, &monitor);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, monitor.w, monitor.h, SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	running = true;
}

void Orbits::event()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
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
		}
		screens[currentScreen].event(&e);
	}
}

void Orbits::draw()
{
	SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xFF);
	SDL_RenderClear(renderer);
	screens[currentScreen].draw();
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

Orbits::Screen::Screen(std::vector<Widget> widgets) : widgets{ widgets } {}

void Orbits::Screen::draw()
{
	for (const Widget& widget : widgets)
		widget.draw(renderer);
}

void Orbits::Screen::event(SDL_Event* e)
{
	for (Widget& widget : widgets)
		widget.event(e);
}