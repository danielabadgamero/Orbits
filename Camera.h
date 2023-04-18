#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

#include "Planet.h"

class Camera
{
private:
	SDL_FRect viewport{};
	int focusedPlanet{};
public:
	void move(int, int);
	void focus();
	void zoom(double);
	int& getFocus();
	SDL_FRect getViewport();
	Camera(SDL_FPoint = {});
};

#endif