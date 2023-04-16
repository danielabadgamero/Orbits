#include <vector>
#include <string>
#include <cmath>

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

	font = TTF_OpenFont("C:\\Windows\\fonts\\AGENCYR.TTF", 32);
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
			case SDL_SCANCODE_SPACE:
				if (selectedValue == 3)
					selectedValue = 0;
				else
					selectedValue++;
				break;
			case SDL_SCANCODE_BACKSPACE:
				if (planets.size())
					planets.pop_back();
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
		{
			const Uint8* keyState{ SDL_GetKeyboardState(NULL) };
			if (selectedValue)
				values[selectedValue] += keyState[SDL_SCANCODE_LSHIFT] ? static_cast<double>(e.wheel.y) * 10 : e.wheel.y;
			else if (keyState[SDL_SCANCODE_LSHIFT])
				decPosition += e.wheel.y;
			else
				values[selectedValue] += e.wheel.y * std::pow(10, decPosition);

			if (values[selectedValue] < 0)
				values[selectedValue] = 0;
			if (selectedValue && values[selectedValue] > 0xff)
				values[selectedValue] = 0xff;
			break;
		}
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse.x, &mouse.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT && !dragging)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);
				dragging = true;
				mouseDown = { static_cast<float>(mouse.x), static_cast<float>(mouse.y) };
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);
				dragging = false;
				planets.push_back(Planet{ values[0], mouseDown, { mouseDown.x - mouse.x, mouseDown.y - mouse.y }, { (Uint8)values[1], (Uint8)values[2], (Uint8)values[3] } });
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

	for (Planet& planet : planets)
	{
		planet.move(dt, planets, &monitor);
		planet.draw(renderer, planetTexture);
	}

	std::string text{};
	switch (selectedValue)
	{
	case 0:
		text = "Mass: ";
		break;
	case 1:
		text = "R: ";
		break;
	case 2:
		text = "G: ";
		break;
	case 3:
		text = "B: ";
		break;
	}

	text.append(selectedValue ? std::to_string(static_cast<int>(values[selectedValue])) : std::to_string(values[selectedValue]));
	SDL_Color color{ 0xff, 0xff, 0xff };
	if (selectedValue)
		color = { (Uint8)values[1], (Uint8)values[2], (Uint8)values[3] };
	else
		text.append(" (" + std::to_string(decPosition) + ")");
	SDL_Surface* surface{ TTF_RenderText_Blended(font, text.c_str(), color) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(renderer, surface) };

	SDL_Rect rect{};
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	rect.x = (monitor.w - rect.w) / 2;
	SDL_RenderCopy(renderer, texture, NULL, &rect);

	if (dragging)
	{
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderDrawLine(renderer, static_cast<int>(mouseDown.x), static_cast<int>(mouseDown.y), mouse.x, mouse.y);
	}

	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void Orbits::quit()
{
	TTF_CloseFont(font);

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