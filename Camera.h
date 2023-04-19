#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

class Camera
{
private:
	SDL_FRect viewport{};
	int focusedPlanet{};
	double zoomFactor{ 1 };
	SDL_FPoint savedPos{};
public:
	void move(int, int);
	void focus();
	void zoom();
	void savePos(SDL_Point);
	double& getZoomFactor();
	int& getFocus();
	SDL_FRect getViewport();
	Camera(SDL_FPoint = {});
};

#endif