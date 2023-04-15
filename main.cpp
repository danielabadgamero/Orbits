#include "OrbitsCore.h"

int main(int, char**)
{
	Orbits::init("Orbits++");

	while (Orbits::running)
	{
		Orbits::event();
		Orbits::draw();
	}

	Orbits::quit();

	return 0;
}