#include <cmath>
#include <vector>
#include <algorithm>

#include <SDL.h>

#include "Planet.h"

static double dist(SDL_FPoint a, SDL_FPoint b)
{
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

Planet::Planet(double mass, SDL_FPoint pos, SDL_FPoint vel, SDL_Color color) : mass{ mass }, pos{ pos }, vel{ vel }, color { color }
{
	rect.w = rect.h = 10;
}

void Planet::move(double dt, std::vector<Planet>& planets, SDL_DisplayMode* monitor)
{
	double ax{};
	double ay{};

	for (const Planet& planet : planets)
		if (dist(planet.pos, pos) > 10)
		{
			double a{ G * planet.mass / std::pow(dist(planet.pos, pos), 2) };
			ax += a / dist(planet.pos, pos) * (planet.pos.x - pos.x);
			ay += a / dist(planet.pos, pos) * (planet.pos.y - pos.y);
		}
		else if (dist(planet.pos, pos))
		{
			
		}
	
	vel.x += static_cast<float>(ax * dt);
	vel.y += static_cast<float>(ay * dt);

	if (pos.x + vel.x * dt < 0 || pos.x + vel.x * dt > monitor->w - 5)
		vel.x *= -1;
	if (pos.y + vel.y * dt < 0 || pos.y + vel.y * dt > monitor->h - 5)
		vel.y *= -1;

	pos.x += static_cast<float>(vel.x * dt);
	pos.y += static_cast<float>(vel.y * dt);

	places.push_back(pos);
	if (places.size() > 10000)
		places.erase(places.begin());
}

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
	rect.x = static_cast<int>(pos.x) - rect.w / 2;
	rect.y = static_cast<int>(pos.y) - rect.h / 2;

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