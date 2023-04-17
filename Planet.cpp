#include <cmath>
#include <algorithm>
#include <vector>

#include <SDL.h>

#include "Planet.h"

static double distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

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

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture, double fov, double dist, double pitch, double yaw, SDL_DisplayMode* monitor, const Planet* const focus) const
{
	double x{ dist * sin(yaw * M_PI / 180) - focus->pos.x };
	double y{ dist * cos(yaw * M_PI / 180) - focus->pos.y };
	double z{ -dist * sin(pitch * M_PI / 180) }; z;

	double sideA{ distance(x, y, focus->pos.x, focus->pos.y) };
	double b{ distance(x, y, pos.x, pos.y) };
	double c{ distance(focus->pos.x, focus->pos.y, pos.x, pos.y) };
	double gamma{ acos((pow(sideA, 2) + pow(b, 2) - pow(c, 2)) / (2 * sideA * b)) * 180 / M_PI };
	double alpha{ fov / 2 - gamma };

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

Planet* Planet::getParent() const
{
	return parent;
}