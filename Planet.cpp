#include <cmath>
#include <algorithm>

#include <SDL.h>

#include "Planet.h"

Planet::Planet(Planet* parentPlanet, double mass, int radius, double semiMajor, double period, SDL_Color color)
	: m{ mass }, color{ color }, r{ radius }, a{ semiMajor }
{
	T = period * 27.3217;
	parent = parentPlanet;
}

void Planet::move(double dt)
{
	if (!parent)
		return;
	epoch += dt;
	while (epoch > T)
		epoch -= T;
	double angle{ 2 * M_PI * epoch / T };
	pos.x = static_cast<float>(a * cos(angle));
	pos.y = static_cast<float>(a * sin(angle));
}

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture, double zoom, SDL_Point offset)
{
	SDL_Rect rect{ -offset.x, -offset.y, 
		std::clamp(static_cast<int>(zoom * r * 2), 10, INT_MAX),
		std::clamp(static_cast<int>(zoom * r * 2), 10, INT_MAX) };

	if (parent)
	{
		rect.x = static_cast<int>(zoom * pos.x + zoom * parent->pos.x) - offset.x;
		rect.y = static_cast<int>(zoom * pos.y + zoom * parent->pos.y) - offset.y;
	}

	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Point Planet::getPos(double zoom)
{
	return { static_cast<int>(zoom * pos.x), static_cast<int>(zoom * pos.y) };
}