#include <cmath>
#include <algorithm>
#include <vector>

#include <SDL.h>

#include "Planet.h"

Planet::Planet(Planet* parentPlanet, double mass, int radius, double semiMajor, double eccentricity, SDL_Color color)
	: m{ mass }, color{ color }, r{ radius }, a{ semiMajor }, e{ eccentricity }
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

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture, double fov, double dist, double pitch, double yaw, SDL_DisplayMode* monitor)
{
	double x{ pos.x - dist * sin(yaw) * cos(pitch) };
	double y{ pos.y - dist * cos(yaw) * cos(pitch) };
	double z{ -dist * sin(pitch) }; z;

	double beta{ atan((pos.y - y) / (pos.x - x)) };
	double alpha{ yaw - beta - fov / 2 };

	SDL_Rect rect
	{
		static_cast<int>(alpha / fov * monitor->w),
		static_cast<int>(monitor->h / 2),
		std::clamp(static_cast<int>(r / dist * 2), 10, INT_MAX),
		std::clamp(static_cast<int>(r / dist * 2), 10, INT_MAX)
	};
	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;

	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

Planet* Planet::getParent()
{
	return parent;
}