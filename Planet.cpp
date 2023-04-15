#include <cmath>

#include <SDL.h>

#include "Planet.h"

Planet::Planet(Planet* parent, double mass, double initSpeed, double radius, SDL_Color color) : parent{ parent }, mass{ mass }, color{ color }
{
	pos.x = radius;
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

void Planet::draw(SDL_Renderer* renderer, SDL_Texture* texture, double zoom)
{

}