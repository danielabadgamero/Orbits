#include <SDL.h>

#include "OrbitsCore.h"
#include "Planet.h"
#include "Camera.h"

Camera::Camera(SDL_FPoint bounds)
{
	viewport.w = bounds.x;
	viewport.h = bounds.y;
	viewport.x -= viewport.w / 2;
	viewport.y -= viewport.h / 2;
}

void Camera::move(int x, int y)
{
	viewport.x -= static_cast<float>(x) / Orbits::monitor.w * viewport.w;
	viewport.y -= static_cast<float>(y) / Orbits::monitor.h * viewport.h;
}

void Camera::focus()
{
	viewport.x = Orbits::planets[focusedPlanet]->getPos().x;
	viewport.y = Orbits::planets[focusedPlanet]->getPos().y;
	viewport.x -= viewport.w / 2;
	viewport.y -= viewport.h / 2;
}

void Camera::zoom(double wheel)
{
	if (wheel > 0)
	{
		viewport.w *= 0.8f;
		viewport.h *= 0.8f;
	}
	else
	{
		viewport.w /= 0.8f;
		viewport.h /= 0.8f;
	}
}

SDL_FRect Camera::getViewport()
{
	return viewport;
}

int& Camera::getFocus()
{
	return focusedPlanet;
}