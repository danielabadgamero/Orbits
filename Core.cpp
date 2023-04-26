#include <vector>
#include <chrono>

#include "Core.h"

void Orbits::init(const char* title)
{
	running = true;
}

void Orbits::handleEvents()
{
}

void Orbits::draw()
{
	prevTime = currTime;
	currTime = std::chrono::high_resolution_clock::now();
	double dt{ std::chrono::duration_cast<std::chrono::seconds>(currTime - prevTime).count() };

	for (Planet*& planet : planets)
		if (planet)
			planet->move(dt);
}

void Orbits::quit()
{
}