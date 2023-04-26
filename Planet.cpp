#include <numbers>

#include "Planet.h"

Planet::Planet(Planet* parentPlanet, double mass, int radius, double semiMajor, double eccentricity)
	: m{ mass }, r{ radius }, a{ semiMajor }, e{ eccentricity }
{
	parent = parentPlanet;
	if (!parent) return;
	double T{ 2 * std::numbers::pi * sqrt(pow(a, 3) / (G * parent->m)) };
	n = 2 * std::numbers::pi / T;
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
}

void Planet::draw() const
{
}

Planet* Planet::getParent() const
{
	return parent;
}