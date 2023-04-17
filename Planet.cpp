#include <cmath>
#include <vector>
#include <algorithm>

#include <SDL.h>

#include "Planet.h"

static double dist(SDL_FPoint a, SDL_FPoint b)
{
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

Planet::Planet(double mass, int radius, double dist, double initSpeed, SDL_Color color) : mass{ mass }, color{ color }, radius{ radius }
{
	pos.x = static_cast<float>(dist);
	vel.y = static_cast<float>(initSpeed);
}

void Planet::move(double dt, std::vector<Planet>& planets)
{
	double ax{};
	double ay{};

	for (const Planet& planet : planets)
		if (dist(planet.pos, pos))
		{
			double a{ G * planet.mass / std::pow(dist(planet.pos, pos), 2) };
			ax += a / dist(planet.pos, pos) * (planet.pos.x - pos.x);
			ay += a / dist(planet.pos, pos) * (planet.pos.y - pos.y);
		}
	
	vel.x += static_cast<float>(ax * dt);
	vel.y += static_cast<float>(ay * dt);

	pos.x += static_cast<float>(vel.x * dt);
	pos.y += static_cast<float>(vel.y * dt);
}

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture, double zoom, SDL_Point offset)
{
	SDL_Rect rect
	{
		static_cast<int>(zoom * pos.x) - offset.x,
		static_cast<int>(zoom * pos.y) - offset.y,
		std::clamp(static_cast<int>(zoom * radius * 2), 10, 2000),
		std::clamp(static_cast<int>(zoom * radius * 2), 10, 2000)
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