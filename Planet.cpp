#include <cmath>
#include <algorithm>

#include <SDL.h>

#include "Planet.h"

static constexpr double G{ 6.6743e-11 };

Planet::Planet(Planet* parentPlanet, double mass, int radius, double semiMajor, SDL_Color color)
	: m{ mass }, color{ color }, r{ radius }, a{ semiMajor }
{
	parent = parentPlanet;
	if (parent)
		T = sqrt( pow(a, 3) / G * parent->m);
}

void Planet::move(double dt)
{
	if (!parent)
		return;
	epoch += dt;

	double E{ M };
	while (true)
	{
		const double dE{ (E - e * sin(E) - M) / (1 - e * cos(E)) };
		E -= dE;
		if (abs(dE) < 1e-6) break;
	}
}

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture, double zoom, SDL_Point offset)
{
	SDL_Rect rect
	{
		rect.x = static_cast<int>(zoom * pos.x) - offset.x,
		rect.y = static_cast<int>(zoom * pos.y) - offset.y,
		std::clamp(static_cast<int>(zoom * r * 2), 10, INT_MAX),
		std::clamp(static_cast<int>(zoom * r * 2), 10, INT_MAX)
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