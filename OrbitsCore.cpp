#include <vector>
#include <string>
#include <algorithm>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>

#include "OrbitsCore.h"

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

	camera.offset.x -= monitor.w / 2.0f;
	camera.offset.y -= monitor.h / 2.0f;

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
				if (focus + 1 == total_planets)
					focus = 0;
				else
					focus++;
				break;
			case SDL_SCANCODE_DOWN:
				if (focus == 0)
					focus = total_planets - 1;
				else
					focus--;
				break;
			case SDL_SCANCODE_SPACE:
				focus = 0;
				break;
			case SDL_SCANCODE_PERIOD:
				if (timeWarp < 1000000000)
					timeWarp *= 10;
				break;
			case SDL_SCANCODE_COMMA:
				if (timeWarp > 9)
					timeWarp /= 10;
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
			savedPos = mouse;
			if (e.wheel.y > 0)
				camera.zoomSpeed *= 1.05;
			else
				camera.zoomSpeed /= 1.05;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse.x, &mouse.y);
			if (focus)
				break;
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LEFT)
			{
				camera.offset.x -= static_cast<float>(e.motion.xrel);
				camera.offset.y -= static_cast<float>(e.motion.yrel);
			}
			break;
		}
}

void Orbits::draw()
{
	prevTime = currTime;
	currTime = static_cast<double>(SDL_GetTicks64()) / 1000.0;
	double dt{ currTime - prevTime };

	camera.zoom *= camera.zoomSpeed;
	if (camera.zoomSpeed > 1)
	{
		camera.offset.x *= static_cast<float>(camera.zoomSpeed);
		camera.offset.y *= static_cast<float>(camera.zoomSpeed);
		camera.offset.x += static_cast<float>(savedPos.x * (camera.zoomSpeed - 1));
		camera.offset.y += static_cast<float>(savedPos.y * (camera.zoomSpeed - 1));
	}
	else if (camera.zoomSpeed < 1)
	{
		camera.offset.x += static_cast<float>(savedPos.x * (camera.zoomSpeed - 1));
		camera.offset.y += static_cast<float>(savedPos.y * (camera.zoomSpeed - 1));
		camera.offset.x *= static_cast<float>(camera.zoomSpeed);
		camera.offset.y *= static_cast<float>(camera.zoomSpeed);
	}
	camera.zoomSpeed = pow(camera.zoomSpeed, 0.9);
	if (abs(camera.zoomSpeed - 1) < 1e-3)
		camera.zoomSpeed = 1;

	for (int i{}; i != total_planets; i++)
	{
		planets[i]->move(dt * timeWarp);
		if (focus == i && focus)
			camera.offset = { planets[i]->getPos(camera.zoom).x - monitor.w / 2, planets[i]->getPos(camera.zoom).y - monitor.h / 2 };
	}

	SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xff);
	SDL_RenderClear(renderer);

	for (int i{}; i != total_planets; i++)
		if (focus || i == sun || planets[i]->getParent() == planets[sun] || camera.zoom >= 1e-6)
			planets[i]->draw(renderer, images[i], camera.zoom, camera.offset);

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
	planets[sun] = new Planet{ NULL, 1.989e30, 696340000, 0, 0, 0 };
	planets[mercury] = new Planet{ planets[sun], 0.33010e24, 2440500, 57.909e9, 0.2056, 252.25084 };
	planets[venus] = new Planet{ planets[sun], 4.8673e24, 6051800, 108.210e9, 0.0068, 181.97973 };
	planets[earth] = new Planet{ planets[sun], 5.9722e24, 6378137, 149.598e9, 0.0167, 100.46435 };
	planets[moon] = new Planet{ planets[earth], 0.07346e24, 1738100, 0.3844e9, 0.0549, 0 };
	planets[mars] = new Planet{ planets[sun], 0.64169e24, 3396200, 227.956e9, 0.0935, 355.45332 };
	planets[phobos] = new Planet{ planets[mars], 10.6e15, 13000, 9378e3, 0.0151, 0 };
	planets[deimos] = new Planet{ planets[mars], 2.4e15, 7800, 23459e3, 0.0005, 0 };
	planets[jupiter] = new Planet{ planets[sun], 1898.13e24, 71492000, 778.479e9, 0.0487, 34.40438 };
	planets[io] = new Planet{ planets[jupiter], 893.2e20, 1821500, 421.8e6, 0.004, 0 };
	planets[europa] = new Planet{ planets[jupiter], 480e20, 1560800, 671.1e6, 0.009, 0 };
	planets[ganymede] = new Planet{ planets[jupiter], 1481.9e20, 2631200, 1070.4e6, 0.001, 0 };
	planets[callisto] = new Planet{ planets[jupiter], 1075.9e20, 2410300, 1882.7e6, 0.007, 0 };
	planets[saturn] = new Planet{ planets[sun], 568.32e24, 60268000, 1432.041e9, 0.0520, 49.94432 };
	planets[uranus] = new Planet{ planets[sun], 86.811e24, 25559000, 2867.043e9, 0.0469, 313.23218 };
	planets[neptune] = new Planet{ planets[sun], 102.409e24, 24764000, 4514.953e9, 0.0097, 304.88003 };

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

static void loadImage(Orbits::Planets name)
{
	std::vector<char> img{ getImage(Orbits::planetNames[name])};
	SDL_RWops* data{ SDL_RWFromMem(img.data(), static_cast<int>(img.size())) };
	Orbits::images[name] = IMG_LoadPNG_RW(data);
	SDL_RWclose(data);
}

int Orbits::loadImages(void*)
{
	for (int i{}; i != total_planets; i++)
		loadImage(static_cast<Planets>(i));

	imageLoadThread.done = true;
	return 0;
}