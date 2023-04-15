#include <cmath>

#include <SDL.h>

#include "Planet.h"

Planet::Planet(Planet* parent, double mass, double radius, double dist, double initSpeed, SDL_Color color) : parent{ parent }, mass{ mass }, color{ color }, radius{ radius }
{
	pos.x = dist;
	vel.y = initSpeed;
}

void Planet::move(double dt, std::vector<Planet>& planets)
{
	double totalMass{};
	SDL_FPoint CG{};
	for (const Planet& planet : planets)
	{
		CG.x += planet.pos.x * planet.mass;
		CG.y += planet.pos.y * planet.mass;
		totalMass += planet.mass;
	}
	CG.x /= totalMass;
	CG.y /= totalMass;

	vel.x += G * parent->mass / std::pow(CG.x - pos.x, 2);
	vel.y += G * parent->mass / std::pow(CG.y - pos.y, 2);

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
}

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture, int zoom)
{
	double zoomFactor{ 1.0 / static_cast<double>(zoom) };
	SDL_Rect rect
	{
		static_cast<int>(zoomFactor * pos.x),
		static_cast<int>(zoomFactor * pos.y),
		static_cast<int>(zoomFactor * radius) * 2,
		static_cast<int>(zoomFactor * radius) * 2
	};
	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}