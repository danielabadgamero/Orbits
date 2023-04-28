#include "Core.h"

int main(int, char**)
{
	Core::init("Orbits++");

	while (Core::running)
	{
		Core::handleEvents();
		Core::draw();
	}

	Core::quit();

	return 0;
}