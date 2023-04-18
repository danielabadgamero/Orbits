#include <SDL.h>

#include "Planet.h"
#include "Camera.h"

Camera::Camera(SDL_FPoint bounds)
{
	viewport.w = bounds.x;
	viewport.h = bounds.y;
	viewport.x -= viewport.w / 2;
	viewport.y -= viewport.h / 2;
}

void Camera::move(int x, int y, SDL_DisplayMode* monitor)
{
	viewport.x += static_cast<float>(x) / monitor->w * viewport.w;
	viewport.y += static_cast<float>(y) / monitor->h * viewport.h;
}

void Camera::focus()
{

}

void Camera::zoom(double wheel)
{
	wheel;
}

SDL_FRect Camera::getViewport()
{
	return viewport;
}

Planet*& Camera::getFocus()
{
	return focusedPlanet;
}