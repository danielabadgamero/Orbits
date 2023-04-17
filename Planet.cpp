#include <cmath>
#include <vector>
#include <algorithm>

#include <SDL.h>

#include "Planet.h"

Planet::Planet(Planet* parent, double mass, int radius, double semiMajor, double period, SDL_Color color)
	: m{ mass }, color{ color }, r{ radius }, parent{ parent }, a{ semiMajor }
{
	T = period * 27.3217;
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
	SDL_Rect rect
	{
		static_cast<int>(zoom * pos.x + (parent ? parent->pos.x : 0)) - offset.x,
		static_cast<int>(zoom * pos.y + (parent ? parent->pos.y : 0)) - offset.y,
		std::clamp(static_cast<int>(zoom * r * 2), 10, 2000),
		std::clamp(static_cast<int>(zoom * r * 2), 10, 2000)
	};

	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Point Planet::getPos(double zoom)
{
	return { static_cast<int>(zoom * pos.x), static_cast<int>(zoom * pos.y) };
}