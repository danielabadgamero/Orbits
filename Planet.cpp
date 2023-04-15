#include <SDL.h>

#include "Planet.h"

Planet::Planet(Planet* parent, double mass, double initSpeed, double radius, SDL_Color color) : parent{ parent }, mass{ mass }, color{ color }
{
	pos.x = radius;
	vel.y = initSpeed;
}

void Planet::move(double dt, std::vector<Planet> planets)
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
}