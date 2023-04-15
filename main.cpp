#include "OrbitsCore.h"

int main(int, char**)
{
	Orbits::init("Orbits++");

	while (Orbits::running)
	{
		Orbits::handleEvents();
		Orbits::draw();
	}

	Orbits::quit();

	return 0;
}