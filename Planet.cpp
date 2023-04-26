#include <numbers>
#include <algorithm>
#include <vector>

#include "Planet.h"

static double distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

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