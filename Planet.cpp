#include <cmath>
#include <vector>
#include <algorithm>

#include <SDL.h>

#include "Planet.h"

SDL_FPoint operator+(SDL_FPoint A, SDL_FPoint B)
{
	return { A.x + B.x, A.y + B.y };
}

static double dist(SDL_FPoint a, SDL_FPoint b)
{
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

Planet::Planet(double mass, int radius, SDL_FPoint pos, SDL_FPoint vel, SDL_Color color) : mass{ mass }, pos{ pos }, vel{ vel }, color { color }
{
	rect.w = rect.h = radius * 2;
}

void Planet::move(double dt, std::vector<Planet>& planets, SDL_DisplayMode* monitor)
{
	double ax{};
	double ay{};

	for (Planet& planet : planets)
		if (dist(planet.pos + planet.vel, pos + vel) > (planet.rect.w + rect.w) / 2.0)
		{
			double a{ G * planet.mass / std::pow(dist(planet.pos, pos), 2) };
			ax += a / dist(planet.pos, pos) * (planet.pos.x - pos.x);
			ay += a / dist(planet.pos, pos) * (planet.pos.y - pos.y);
		}
		else if (dist(planet.pos, pos))
		{
			double dir{};
			dir;
		}
	
	vel.x += static_cast<float>(ax * dt);
	vel.y += static_cast<float>(ay * dt);

	if (pos.x + vel.x * dt < rect.w / 2 || pos.x + vel.x * dt > monitor->w - rect.w / 2)
		vel.x *= -1;
	if (pos.y + vel.y * dt < rect.h / 2 || pos.y + vel.y * dt > monitor->h - rect.h / 2)
		vel.y *= -1;

	pos.x += static_cast<float>(vel.x * dt);
	pos.y += static_cast<float>(vel.y * dt);

	places.push_back(pos);
	if (places.size() > 1000)
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