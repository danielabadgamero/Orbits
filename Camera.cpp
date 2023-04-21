#include <cmath>

#include <SDL.h>

#include "OrbitsCore.h"
#include "Planet.h"
#include "Camera.h"
#include "Vessel.h"

Camera::Camera(SDL_FPoint bounds)
{
	viewport.w = bounds.x;
	viewport.h = bounds.y;
}

void Camera::move(int x, int y)
{
	viewport.x -= static_cast<float>(x) / Orbits::monitor.w * viewport.w;
	viewport.y -= static_cast<float>(y) / Orbits::monitor.h * viewport.h;
}

void Camera::focus()
{
	if (focusedPlanet == totalPlanets)
	{
		// focus vessel
	}
	else
	{
		viewport.x = Orbits::planets[focusedPlanet]->getPos().x;
		viewport.y = Orbits::planets[focusedPlanet]->getPos().y;
	}
}

void Camera::zoom()
{
	zoomFactor = pow(zoomFactor, 0.8f);

	if (zoomFactor > 1.0f)
	{
		viewport.x += static_cast<float>((savedPos.x - Orbits::monitor.w / 2.0f) / Orbits::monitor.w * viewport.w * (1 - zoomFactor));
		viewport.y += static_cast<float>((savedPos.y - Orbits::monitor.h / 2.0f) / Orbits::monitor.h * viewport.h * (1 - zoomFactor));
		viewport.w *= static_cast<float>(zoomFactor);
		viewport.h *= static_cast<float>(zoomFactor);
	}
	else if (zoomFactor < 1.0f)
	{
		viewport.w *= static_cast<float>(zoomFactor);
		viewport.h *= static_cast<float>(zoomFactor);
		viewport.x += static_cast<float>((savedPos.x - Orbits::monitor.w / 2.0f) / Orbits::monitor.w * viewport.w * (1 - zoomFactor));
		viewport.y += static_cast<float>((savedPos.y - Orbits::monitor.h / 2.0f) / Orbits::monitor.h * viewport.h * (1 - zoomFactor));
	}
}

void Camera::savePos(SDL_Point mouse)
{
	savedPos.x = static_cast<float>(mouse.x);
	savedPos.y = static_cast<float>(mouse.y);
}

double& Camera::getZoomFactor()
{
	return zoomFactor;
}

SDL_FRect Camera::getViewport()
{
	return viewport;
}

int& Camera::getFocus()
{
	return focusedPlanet;
}