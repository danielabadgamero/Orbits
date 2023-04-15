#include <cmath>

#include <SDL.h>

#include "Planet.h"

Planet::Planet(double mass, int radius, double dist, double initSpeed, SDL_Color color) : mass{ mass }, color{ color }, radius{ radius }
{
	pos.x = static_cast<float>(dist);
	vel.y = static_cast<float>(initSpeed);
}

void Planet::move(double dt, std::vector<Planet>& planets)
{
	double totalMass{};
	SDL_FPoint CG{};
	for (const Planet& planet : planets)
	{
		CG.x += static_cast<float>(planet.pos.x * planet.mass);
		CG.y += static_cast<float>(planet.pos.y * planet.mass);
		totalMass += planet.mass;
	}
	CG.x /= static_cast<float>(totalMass);
	CG.y /= static_cast<float>(totalMass);

	if (CG.x - pos.x != 0)
		vel.x += static_cast<float>(dt * G * totalMass / std::pow(CG.x - pos.x, 2));
	if (CG.y - pos.y != 0)
		vel.y += static_cast<float>(dt * G * totalMass / std::pow(CG.y - pos.y, 2));

	pos.x += static_cast<float>(vel.x * dt);
	pos.y += static_cast<float>(vel.y * dt);
}

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture, double zoom, SDL_Point offset)
{
	double zoomFactor{ 1.0 / static_cast<double>(zoom) };
	SDL_Rect rect
	{
		static_cast<int>(zoomFactor * pos.x) + offset.x,
		static_cast<int>(zoomFactor * pos.y) + offset.y,
		static_cast<int>(zoomFactor * radius) * 2,
		static_cast<int>(zoomFactor * radius) * 2
	};

	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}