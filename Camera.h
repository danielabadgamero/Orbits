#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

#include "Planet.h"

class Camera
{
private:
	SDL_FRect viewport{};
	Planet* focusedPlanet{};
public:
	void move(int, int, SDL_DisplayMode*);
	void focus();
	void zoom(double);
	Planet*& getFocus();
	SDL_FRect getViewport();
	Camera(SDL_FPoint = {});
};

#endif