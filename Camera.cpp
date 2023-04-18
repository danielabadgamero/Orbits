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

void Camera::move(int x, int y)
{

}

void Camera::focus()
{

}

void Camera::zoom(double wheel)
{

}

Planet*& Camera::getFocus()
{
	return focusedPlanet;
}