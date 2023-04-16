#include <cmath>
#include <vector>
#include <algorithm>

#include <SDL.h>

#include "Planet.h"

static double dist(SDL_Point a, SDL_Point b)
{
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

Planet::Planet(double mass, SDL_Point pos, SDL_Point vel, SDL_Color color) : mass{ mass }, pos{ pos }, vel{ vel }, color { color }
{
	rect.x = pos.x - 5;
	rect.y = pos.y - 5;
	rect.w = rect.h = 10;
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

	places.push_back(pos);
	if (places.size() > 2000)
		places.erase(places.begin());
}

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture)
{	
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	for (int i{}; i < (int)places.size() - 1; i++)
		SDL_RenderDrawLine(renderer, static_cast<int>(places[i].x), static_cast<int>(places[i].y), static_cast<int>(places[i + 1].x), static_cast<int>(places[i + 1].y));
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Point Planet::getPos(double zoom)
{
	return { static_cast<int>(zoom * pos.x), static_cast<int>(zoom * pos.y) };
}