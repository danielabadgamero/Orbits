#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>
#include <sstream>

#include <SDL.h>

#include "OrbitsCore.h"
#include "Planet.h"

Planet::Planet(Planet* parentPlanet, double mass, int radius, double semiMajor, double eccentricity)
	: m{ mass }, r{ radius }, a{ semiMajor }, e{ eccentricity }
{
	parent = parentPlanet;
	if (!parent) return;
	double T{ 2 * M_PI * sqrt(pow(a, 3) / (G * parent->m)) };
	n = 2 * M_PI / T;
}

void Planet::move(double dt)
{
	if (!parent)
		return;

	M += dt * n;
	double E{ M };
	while (true)
	{
		const double dE{ (E - e * sin(E) - M) / (1 - e * cos(E)) };
		E -= dE;
		if (abs(dE) < 1e-6) break;
	}

	pos.x = static_cast<float>(a * (cos(E) - e)) + parent->pos.x;
	pos.y = static_cast<float>(a * sin(E) * sqrt(1 - pow(e, 2))) + parent->pos.y;
}

void Planet::draw(SDL_Surface* surface, SDL_FRect viewport)
{
	SDL_Rect rect
	{
		static_cast<int>((pos.x - viewport.x) / viewport.w * Orbits::monitor.w),
		static_cast<int>((pos.y - viewport.y) / viewport.h * Orbits::monitor.h),
		std::clamp(static_cast<int>(r / viewport.w) * Orbits::monitor.w, 10, INT_MAX),
		std::clamp(static_cast<int>(r / viewport.h) * Orbits::monitor.h, 10, INT_MAX),
	};
	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;

	SDL_Texture* texture{ SDL_CreateTextureFromSurface(Orbits::renderer, surface) };
	SDL_RenderCopy(Orbits::renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
}

Planet* Planet::getParent()
{
	return parent;
}

SDL_FPoint Planet::getPos()
{
	return pos;
}