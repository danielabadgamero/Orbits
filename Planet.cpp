#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>
#include <sstream>

#include <SDL.h>

#include "Planet.h"
#include "Camera.h"

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

void Planet::draw(SDL_Renderer* renderer, SDL_Surface* surface, Camera viewport)
{
	SDL_Rect rect
	{

	};
	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;

	SDL_Texture* texture{ SDL_CreateTextureFromSurface(renderer, surface) };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
}

Planet* Planet::getParent()
{
	return parent;
}